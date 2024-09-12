// No Copyright!

// Game Includes
#include "Character/Cpp_Character_Player.h"
#include "Core/Cpp_PC_Player.h"
#include "Core/Cpp_PS_Player.h"
#include "UI/Cpp_HUD_RPG.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "AbilitySystem/DataAssets/Cpp_DA_LevelUpInfo.h"
#include "Cpp_AllGameplayTags.h"
#include "AbilitySystem/Debuff/Cpp_NC_Debuff.h"


// Engine Includes
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACpp_Character_Player::ACpp_Character_Player() {
	// Setup Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Setup Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;

	// Setup Camera Defaults
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setup Combat Defaults
	CharacterClass = ECharacterClass::Mage;

	// Setup Level Up Niagara Component
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->SetAutoActivate(false);			
}

void ACpp_Character_Player::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	// Initialize the Gameplay Ability System ONLY if we are the SERVER	
	InitAbilityActorInfo();
	AddCharacterAbilities();
}
void ACpp_Character_Player::OnRep_PlayerState() {
	Super::OnRep_PlayerState();

	// Initialize the Gameplay Ability System ONLY if we are the CLIENT
	InitAbilityActorInfo();
}

int32 ACpp_Character_Player::GetPlayerLevel_Implementation() const {
	// Get the Player Level from the Player State
	const ACpp_PS_Player* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);	
	return TempPlayerState->GetPlayerLevel();
}

void ACpp_Character_Player::UpdateExperience_Implementation(int32 ExperienceToAdd) {
	ACpp_PS_Player* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);

	TempPlayerState->UpdateExperience(ExperienceToAdd);
}
void ACpp_Character_Player::UpdatePlayerLevel_Implementation(int32 LevelsToAdd) {
	auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	TempPlayerState->UpdateLevel(LevelsToAdd);

	if (UCpp_ASC_Master* TempASC = Cast<UCpp_ASC_Master>(GetAbilitySystemComponent())) {
		TempASC->UpdateAbilityStatuses(TempPlayerState->GetPlayerLevel());
	}
}
void ACpp_Character_Player::UpdatePlayerAttributePoints_Implementation(int32 AttributePointsToAdd) {
	auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	TempPlayerState->UpdateAttributePoints(AttributePointsToAdd);
	return;
}
void ACpp_Character_Player::UpdatePlayerSpellPoints_Implementation(int32 SpellPointsToAdd) {
	auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	TempPlayerState->UpdateSpellPoints(SpellPointsToAdd);
	return;
}

int32 ACpp_Character_Player::GetExperience_Implementation() const {
	const auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	return TempPlayerState->GetExperience();	
}
int32 ACpp_Character_Player::GetAttributPointsRewardForLevelUp_Implementation(int32 Level) const {
	const auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	return TempPlayerState->LevelUpInfo->LevelUpInfo[Level].AttributePointsGained;
}
int32 ACpp_Character_Player::GetSpellPointsRewardForLevelUp_Implementation(int32 Level) const {
	const auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);
	return TempPlayerState->LevelUpInfo->LevelUpInfo[Level].SpellPointsGained;
}
int32 ACpp_Character_Player::FindLevelForExperience_Implementation(int32 Experience) const {
	const auto* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);

	return TempPlayerState->LevelUpInfo->FindLevelForExperience(Experience);
}
int32 ACpp_Character_Player::GetAttributePoints_Implementation() const {
	const auto* TempPlayerState = CastChecked<ACpp_PS_Player>(GetPlayerState());
	return TempPlayerState->GetAttributePoints();
}
int32 ACpp_Character_Player::GetSpellPoints_Implementation() const {
	const auto* TempPlayerState = CastChecked<ACpp_PS_Player>(GetPlayerState());
	return TempPlayerState->GetSpellPoints();
}

void ACpp_Character_Player::LevelUp_Implementation() {
	MC_LevelUpNiagara();
}

void ACpp_Character_Player::InitAbilityActorInfo() {
	ACpp_PS_Player* TempPlayerState = Cast<ACpp_PS_Player>(GetPlayerState());
	check(TempPlayerState);

	// Initialize the Gameplay Ability System
	TempPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TempPlayerState, this);

	Cast<UCpp_ASC_Master>(TempPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	// Set Ability System Component & Attribute Set From TempPlayerState
	AbilitySystemComponent = TempPlayerState->GetAbilitySystemComponent();
	AttributeSet = TempPlayerState->GetAttributeSet();
	OnAscRegistredDelegate.Broadcast(AbilitySystemComponent);
	
	// Register when the stun tag changes
	AbilitySystemComponent->RegisterGameplayTagEvent(FAllGameplayTags::Get().Debuff_Shock,
													 EGameplayTagEventType::NewOrRemoved).AddUObject(
														 this, &ACpp_Character_Player::ShockTagChanged);

	if (ACpp_PC_Player* TempPlayerController = Cast<ACpp_PC_Player>(GetController())) {
		// Initialize the HUD
		if (auto TempHUD = Cast<ACpp_HUD_RPG>(TempPlayerController->GetHUD())) {
			TempHUD->InitHUD(TempPlayerController, TempPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Initialize the Default Attributes
	InitializeDefaultAttributes();
}

void ACpp_Character_Player::OnRep_Shocked() {
	if (auto* ASC = Cast<UCpp_ASC_Master>(AbilitySystemComponent)) {
		
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FAllGameplayTags::Get().Player_Block_CursorTrace);
		BlockedTags.AddTag(FAllGameplayTags::Get().Player_Block_Input_Held);
		BlockedTags.AddTag(FAllGameplayTags::Get().Player_Block_Input_Pressed);
		BlockedTags.AddTag(FAllGameplayTags::Get().Player_Block_Input_Released);

		if (bIsShocked) {
			ASC->AddLooseGameplayTags(BlockedTags);	
			// For Client
			ShockDebuffComponent->Activate();
		}
		else {
			ASC->RemoveLooseGameplayTags(BlockedTags);	
			// For Client
			ShockDebuffComponent->Deactivate();
		}
	}

}
void ACpp_Character_Player::OnRep_Burnt() {
	if (bIsBurnt) {
		// For Client
		BurnDebuffComponent->Activate();
	}
	else {
		// For Client
		BurnDebuffComponent->Deactivate();
	}
}

void ACpp_Character_Player::MC_LevelUpNiagara_Implementation() const {
	if (IsValid(LevelUpNiagaraComponent)) {
		// turn niagara towards the camera
		const FRotator TempCameraRotation = CameraComponent->GetComponentRotation();
		LevelUpNiagaraComponent->SetWorldRotation(FRotator(0.0f, TempCameraRotation.Yaw, 0.0f));		
		LevelUpNiagaraComponent->Activate(true);			
		UE_LOG(LogTemp, Warning, TEXT("Level Up Niagara Activated!"));
	}
}

