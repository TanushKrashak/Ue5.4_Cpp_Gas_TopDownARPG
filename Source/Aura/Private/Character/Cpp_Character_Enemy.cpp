// No Copyright!

// Game Includes
#include "Character/Cpp_Character_Enemy.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "UI/Widgets/Cpp_WGT_Master.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "Cpp_AllGameplayTags.h"
#include "Ai/AiC/Cpp_AiC_Master.h"

// Engine Includes
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#define CUSTOM_DEPTH_RED 250

ACpp_Character_Enemy::ACpp_Character_Enemy() {
	// Setup Collision Response To Visibility as Block
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Setup Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UCpp_ASC_Master>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Setup Attributes Set
	AttributeSet = CreateDefaultSubobject<UCpp_ASet_Master>(TEXT("Attribute Set"));

	// Setup Health Bar
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());

	// Setup Character Movement Component
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	BaseWalkSpeed = 250.0f;
}

void ACpp_Character_Enemy::BeginPlay() {
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (HasAuthority()) {
		UCpp_BFL_AbilitySystem::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	// Set Widget Controller for Health Bar
	if (UCpp_WGT_Master* MasterWidget = Cast<UCpp_WGT_Master>(HealthBarWidgetComponent->GetUserWidgetObject())) {
		MasterWidget->SetWidgetController(this);
	}

	UCpp_ASet_Master* AS = CastChecked<UCpp_ASet_Master>(AttributeSet);
	// Bind to Health Changed Event
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	// Bind to Max Health Changed Event
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Bind to Hit React Tag Event
	AbilitySystemComponent->RegisterGameplayTagEvent(FAllGameplayTags::Get().Effect_HitReact,
													 EGameplayTagEventType::NewOrRemoved).AddUObject(
														 this, &ACpp_Character_Enemy::HitReactTagChanged);

	// Broadcast Initial Values
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void ACpp_Character_Enemy::HighlightActor() {		
	// Set Custom Render Depth For Mesh + Weapon
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}
void ACpp_Character_Enemy::UnhighlightActor() {		
	// Set Custom Render Depth For Mesh + Weapon
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void ACpp_Character_Enemy::SetCombatTarget_Implementation(AActor* Target) {
	CombatTarget = Target;
}
AActor* ACpp_Character_Enemy::GetCombatTarget_Implementation() const {
	return CombatTarget;
}

int32 ACpp_Character_Enemy::GetPlayerLevel_Implementation() const {
	return Level;
}
void ACpp_Character_Enemy::Die(const FVector& DeathImpulse) {
	SetLifeSpan(OnDeathLifeSpan);
	
	if (AiControllerRef) {
		AiControllerRef->GetBlackboardComponent()->SetValueAsBool("Dead?", true);
	}
	
	Super::Die(DeathImpulse);
}

void ACpp_Character_Enemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;

	if (AiControllerRef && AiControllerRef->GetBlackboardComponent()) {
		AiControllerRef->GetBlackboardComponent()->SetValueAsBool("HitReacting?", bHitReacting);
	}
}

void ACpp_Character_Enemy::InitAbilityActorInfo() {
	// Init Gameplay Ability Owner
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	Cast<UCpp_ASC_Master>(AbilitySystemComponent)->AbilityActorInfoSet();

	// Register when the stun tag changes
	AbilitySystemComponent->RegisterGameplayTagEvent(FAllGameplayTags::Get().Debuff_Shock,
													 EGameplayTagEventType::NewOrRemoved).AddUObject(
														 this, &ACpp_Character_Enemy::ShockTagChanged);

	if (HasAuthority()) {
		InitializeDefaultAttributes();
	}
	OnAscRegistredDelegate.Broadcast(AbilitySystemComponent);
}
void ACpp_Character_Enemy::InitializeDefaultAttributes() const {
	UCpp_BFL_AbilitySystem::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ACpp_Character_Enemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	// Ai Is Supposed To Only Be Run On Server
	if (!HasAuthority()) {
		return;
	}


	// Setup Ai Controller
	AiControllerRef = Cast<ACpp_AiC_Master>(NewController);
	AiControllerRef->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	AiControllerRef->RunBehaviorTree(BehaviorTree);
	AiControllerRef->GetBlackboardComponent()->SetValueAsBool("HitReacting?", false);
	if (CharacterClass == ECharacterClass::Archer || CharacterClass == ECharacterClass::Mage) {
		AiControllerRef->GetBlackboardComponent()->SetValueAsBool("RangedAttacker?", true);
	}
}

void ACpp_Character_Enemy::ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {

	Super::ShockTagChanged(CallbackTag, NewCount);

	if (AiControllerRef && AiControllerRef->GetBlackboardComponent()) {
		AiControllerRef->GetBlackboardComponent()->SetValueAsBool("Shocked?", bIsShocked);
	}
}

