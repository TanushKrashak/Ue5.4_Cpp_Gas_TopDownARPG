// No Copyright!

// Game Includes
#include "Character/Cpp_Character.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Aura/Aura.h"
#include "Cpp_AllGameplayTags.h"	
#include "AbilitySystem/Debuff/Cpp_NC_Debuff.h"

// Engine Includes
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACpp_Character::ACpp_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup Weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Setup Capsule
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	// Setup Mesh
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Setup Burn Debuff Component
	BurnDebuffComponent = CreateDefaultSubobject<UCpp_NC_Debuff>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetMesh());
	BurnDebuffComponent->DebuffTag = FAllGameplayTags::Get().Debuff_Burn;

	// Setup Shock Debuff Component
	ShockDebuffComponent = CreateDefaultSubobject<UCpp_NC_Debuff>("ShockDebuffComponent");
	ShockDebuffComponent->SetupAttachment(GetMesh());
	ShockDebuffComponent->DebuffTag = FAllGameplayTags::Get().Debuff_Shock;
}

void ACpp_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ACpp_Character::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void ACpp_Character::Die(const FVector& DeathImpulse) {
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MC_HandleDeath(DeathImpulse);
}
bool ACpp_Character::IsDead_Implementation() const {
	return bIsDead;
}
FOnDeath& ACpp_Character::GetOnDeathDelegate() {
	return OnDeathDelegate;
}
AActor* ACpp_Character::GetAvatar_Implementation() {
	return this;
}
ECharacterClass ACpp_Character::GetCharacterClass_Implementation() const {
	return CharacterClass;
}
USkeletalMeshComponent* ACpp_Character::GetWeapon_Implementation() const {
	return Weapon;
}
int32 ACpp_Character::GetCurrentSummonsCount_Implementation() const {
	return CurrentSummonsCount;
}
void ACpp_Character::UpdateSummonsCount_Implementation(const int32 Amount = 1) {
	CurrentSummonsCount += Amount;
}
FVector ACpp_Character::GetSocketLocation_Implementation(const FGameplayTag& MontageTag) {

	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();
   // Weapon Socket Location
	if (MontageTag.MatchesTag(AllTags.CombatSocket_Weapon) && IsValid(Weapon)) {
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	// Left Hand Socket Location
	else if (MontageTag.MatchesTag(AllTags.CombatSocket_LeftHand)) {
		return GetMesh()->GetSocketLocation(LeftHandAttackSocketName);
	}
	// Right Hand Socket Location
	else if (MontageTag.MatchesTag(AllTags.CombatSocket_RightHand)) {
		return GetMesh()->GetSocketLocation(RightHandAttackSocketName);
	}
	// Left Foot Socket Location
	else if (MontageTag.MatchesTag(AllTags.CombatSocket_LeftFoot)) {
		return GetMesh()->GetSocketLocation(LeftFootAttackSocketName);
	}
	// Right Foot Socket Location
	else if (MontageTag.MatchesTag(AllTags.CombatSocket_RightFoot)) {
		return GetMesh()->GetSocketLocation(RightFootAttackSocketName);
	}
	else if (MontageTag.MatchesTag(AllTags.CombatSocket_Tail)) {
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ACpp_Character::GetSocketLocation_Implementation: Montage Tag not recognized!"));
	}
	return Weapon->GetSocketLocation(WeaponTipSocketName);	
}
UAnimMontage* ACpp_Character::GetHitReactMontage_Implementation() {
	return HitReactMontage;
}
TArray<FTaggedMontage> ACpp_Character::GetAttackMontages_Implementation() {
	return AttackMontages;
}
FTaggedMontage ACpp_Character::GetAttackMontageByTag_Implementation(const FGameplayTag MontageTag) const {
	// Find Montage by Tag
	for (const FTaggedMontage& TaggedMontage : AttackMontages) {				
		if (TaggedMontage.MontageTag.MatchesTag(MontageTag)) {
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}
UNiagaraSystem* ACpp_Character::GetHitReactEffect_Implementation() const {
	return HitReactNiagaraEffect;
}
FOnAscRegistred ACpp_Character::GetOnAscRegistredDelegate() {
	return OnAscRegistredDelegate;
}
bool ACpp_Character::GetIsBeingShocked_Implementation() const {
	return bIsBeingShocked;
}
void ACpp_Character::SetIsBeingShocked_Implementation(bool bNewValue) {
	bIsBeingShocked = bNewValue;
}

void ACpp_Character::MC_HandleDeath_Implementation(const FVector& DeathImpulse) {
	// Play Death Sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());

	// Enable Physics on Weapon
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	// Enable Physics on Mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);	
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	// Disable Capsule Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bIsDead = true;

	// Deactive both debuff niagara components
	if (BurnDebuffComponent->IsActive()) {
		BurnDebuffComponent->Deactivate();
	}
	if (ShockDebuffComponent->IsActive()) {
		ShockDebuffComponent->Deactivate();
	}

	OnDeathDelegate.Broadcast(this);
}

UAttributeSet* ACpp_Character::GetAttributeSet() const {
	return AttributeSet;
}

void ACpp_Character::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> effect, float level) const {
	// Mandatory Checks for Ability System Component and Default Primary Attributes
	check(IsValid(GetAbilitySystemComponent()));
	check(effect);

	// Create the Gameplay Effect Context
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	// Create the Gameplay Effect Spec
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(effect, level, ContextHandle);
	// Apply the Gameplay Effect Spec to the Target
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACpp_Character::InitAbilityActorInfo() {

}
void ACpp_Character::InitializeDefaultAttributes() const {
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}

void ACpp_Character::AddCharacterAbilities() {
	UCpp_ASC_Master* ASC = Cast<UCpp_ASC_Master>(GetAbilitySystemComponent());
	if (!HasAuthority()) {
		return;
	}
	// Call AddCharacterAbilities on ASC only if has authority
	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);

}

void ACpp_Character::Dissolve() {
	if (IsValid(DissolveMaterialInstance)) {
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance)) {
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

void ACpp_Character::ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bIsShocked = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsShocked ? 0.0f : BaseWalkSpeed;
	if (bIsShocked) {
		ShockDebuffComponent->Activate();
	}
	else {
		ShockDebuffComponent->Deactivate();
	}
}

void ACpp_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACpp_Character, bIsShocked);
	DOREPLIFETIME(ACpp_Character, bIsBeingShocked);
	DOREPLIFETIME(ACpp_Character, bIsBurnt);
}

void ACpp_Character::OnRep_Shocked() {
	
}
void ACpp_Character::OnRep_Burnt() {

}

