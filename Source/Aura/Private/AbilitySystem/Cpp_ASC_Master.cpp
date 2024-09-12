// No Copyright!

// Game Includes
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Cpp_AllGameplayTags.h"
#include "AbilitySystem/Abilities/Cpp_GameplayAbility_RPG.h"
#include "Aura/RpgLogChannels.h"
#include "Interfaces/Cpp_PlayerInterface.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "AbilitySystem/DataAssets/Cpp_DA_AbilityInfo.h"

// Engine Includes
#include "AbilitySystemBlueprintLibrary.h"

void UCpp_ASC_Master::AbilityActorInfoSet() {
	// Bind to the OnGameplayEffectAppliedDelegate which is called whenever a GameplayEffect is applied to this AbilitySystemComponent
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCpp_ASC_Master::ClientEffectApplied);

}

void UCpp_ASC_Master::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities) {
	for(const TSubclassOf<UGameplayAbility> Ability : StartupAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);		

		if (const UCpp_GameplayAbility_RPG* AbilityRPG = Cast<UCpp_GameplayAbility_RPG>(AbilitySpec.Ability)) {
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityRPG->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAllGameplayTags::Get().Ability_Status_Equipped);
			GiveAbility(AbilitySpec);				
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}
void UCpp_ASC_Master::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities) {
	// Go Through Each Ability in the StartupPassiveAbilities Array
	for (const TSubclassOf<UGameplayAbility> Ability : StartupPassiveAbilities) {		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		// Passives are always active
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UCpp_ASC_Master::AbilityInputTagPressed(const FGameplayTag& InputTag) {
	if (!InputTag.IsValid())
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive()) {				
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
				//break;
			}

		}
	}
}
void UCpp_ASC_Master::AbilityInputTagHeld(const FGameplayTag& InputTag) {
	if (!InputTag.IsValid())
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive()) {
				TryActivateAbility(AbilitySpec.Handle);
				//break;
			}

		}
	}
}
void UCpp_ASC_Master::AbilityInputTagReleased(const FGameplayTag& InputTag) {
	if (!InputTag.IsValid())
		return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive()) {
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}
void UCpp_ASC_Master::ForEachAbility(const FForEachAbility& Delegate) {
	// Lock the Ability List so that it doesn't change while we are iterating over it
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (!Delegate.ExecuteIfBound(AbilitySpec)) {
			UE_LOG(LogRpg, Warning, TEXT("Failed to execute ForEachAbility Delegate in %hs"), __FUNCTION__);
		}
	}	
}

FGameplayTag UCpp_ASC_Master::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) {
	if (AbilitySpec.Ability) {
		for (const FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags) {
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
				return Tag;
		}
	}
	return FGameplayTag::EmptyTag;
}
FGameplayTag UCpp_ASC_Master::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) {
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags) {
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
			return Tag;
	}
	return FGameplayTag::EmptyTag;
}
FGameplayTag UCpp_ASC_Master::GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec) {
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags) {
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Status"))))
			return Tag;
	}
	return FGameplayTag::EmptyTag;
}
FGameplayTag UCpp_ASC_Master::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag) {
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag)) {
		return GetAbilityStatusFromSpec(*Spec);
	}
	return FGameplayTag::EmptyTag;
}
FGameplayTag UCpp_ASC_Master::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag) {
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag)) {
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag::EmptyTag;
}

void UCpp_ASC_Master::UpgradeAttribute(const FGameplayTag& AttributeTag) {
	// check if char is player
	if (GetAvatarActor()->Implements<UCpp_PlayerInterface>()) {
		// check if char has enough attribute points
		if (ICpp_PlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0) {
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

FGameplayAbilitySpec* UCpp_ASC_Master::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag) {
	FScopedAbilityListLock ActiveScopeLock(*this);
	// Go Through Each Ability Spec in the Ability List
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		// Go Through Each Tag in the Ability Tags of the Ability Spec
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags) {
			if (Tag.MatchesTag(AbilityTag)) {	
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}
void UCpp_ASC_Master::UpdateAbilityStatuses(int32 Level) {
	UCpp_DA_AbilityInfo* AbilityInfo = UCpp_BFL_AbilitySystem::GetAbilityInfoDA(GetAvatarActor());

	for (const FAbilityInfo_RPG& Info : AbilityInfo->AllAbilityInfo) {
		
		// Filtering out Abilities that are not available yet or are not valid
		if (Info.AbilityTag.IsValid() == false || Info.LevelRequirement > Level) {
			continue;
		}
			
		// This removes all Eqipped Abilities who might be affected by the Level Change, cuz you dont need to change
			// the status of abilities that are equipped already
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr) {
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			// Set status to Eligible
			AbilitySpec.DynamicAbilityTags.AddTag(FAllGameplayTags::Get().Ability_Status_Eligible);
			GiveAbility(AbilitySpec);
			// This is important to mark the Ability Spec as dirty so that the UI can update for clients
			MarkAbilitySpecDirty(AbilitySpec);

			ClientAbilityStatusChanged(Info.AbilityTag, FAllGameplayTags::Get().Ability_Status_Eligible, 1);
		}
	}
	
}

void UCpp_ASC_Master::ServerSpendPoints_Implementation(const FGameplayTag& AbilityTag) {
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag)) {
		
		if (GetAvatarActor()->Implements<UCpp_PlayerInterface>()) {
			ICpp_PlayerInterface::Execute_UpdatePlayerSpellPoints(GetAvatarActor(), -1);
		}
				
		FGameplayTag StatusTag = GetAbilityStatusFromSpec(*AbilitySpec);

		const FAllGameplayTags AllTags = FAllGameplayTags::Get();
		if (StatusTag.MatchesTag(AllTags.Ability_Status_Eligible)) {
			AbilitySpec->DynamicAbilityTags.RemoveTag(AllTags.Ability_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(AllTags.Ability_Status_Unlocked);
			StatusTag = AllTags.Ability_Status_Unlocked;
		}
		else if (StatusTag.MatchesTag(AllTags.Ability_Status_Equipped) || StatusTag.MatchesTag(AllTags.Ability_Status_Unlocked)) {
			AbilitySpec->Level += 1;
		}
		// Update for the Clients
		ClientAbilityStatusChanged(AbilityTag, StatusTag, AbilitySpec->Level);
		// Force it to replicate right away
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UCpp_ASC_Master::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag) {
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag)) {
		const FGameplayTag& PreviousSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetAbilityStatusFromSpec(*AbilitySpec);

		const FAllGameplayTags AllTags = FAllGameplayTags::Get();
		// If Equipped or Unlocked
		if (StatusTag.MatchesTag(AllTags.Ability_Status_Equipped) || 
			StatusTag.MatchesTag(AllTags.Ability_Status_Unlocked)) {
			// Remove this InputTag (slot) from any ability that has it
			ClearAbilitiesWithSlot(SlotTag);
			// Clear this ability's slot, just in case it has a different slot
			ClearSlot(AbilitySpec);
			// Now, assign this ability to the slot
			AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);

			// If unlocked, set to equipped
			if (StatusTag.MatchesTagExact(AllTags.Ability_Status_Unlocked)) {
				AbilitySpec->DynamicAbilityTags.RemoveTag(AllTags.Ability_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(AllTags.Ability_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, AllTags.Ability_Status_Equipped, SlotTag, PreviousSlot);
	}
}

bool UCpp_ASC_Master::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription) {
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag)) {
		if (const UCpp_GameplayAbility_RPG* AbilityRPG = Cast<UCpp_GameplayAbility_RPG>(AbilitySpec->Ability)) {
			OutDescription = AbilityRPG->GetDescriptionForLevel(AbilitySpec->Level);
			OutNextLevelDescription = AbilityRPG->GetDescriptionForLevel(AbilitySpec->Level + 1, true);
			return true;
		}
	}
	const UCpp_DA_AbilityInfo* AbilityInfo = UCpp_BFL_AbilitySystem::GetAbilityInfoDA(GetAvatarActor());

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAllGameplayTags::Get().Ability_None)) {
		OutDescription = FString();		
	}
	else {
		const int32 Level = AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement;
		OutDescription = UCpp_GameplayAbility_RPG::GetLockedDescription(Level);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UCpp_ASC_Master::ClearSlot(FGameplayAbilitySpec* AbilitySpec) {
	const FGameplayTag SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(SlotTag);
	MarkAbilitySpecDirty(*AbilitySpec);
}
void UCpp_ASC_Master::ClearAbilitiesWithSlot(const FGameplayTag& SlotTag) {
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (AbilityHasSlot(&AbilitySpec, SlotTag)) {
			ClearSlot(&AbilitySpec);
		}
	}
}
bool UCpp_ASC_Master::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag) {
	// Go Through Each Tag in the Tags of the Ability Spec
	for (const FGameplayTag Tag : AbilitySpec->DynamicAbilityTags) {
		if (Tag.MatchesTag(SlotTag)) {
			return true;
		}
	}
	return false;
}

void UCpp_ASC_Master::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag) {
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	// 1 Magnitude cuz we are just adding 1 point
	Payload.EventMagnitude = 1.0f;

	// Update the Attribute Value
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	// Remove 1 Attribute Point From the Player
	if (GetAvatarActor()->Implements<UCpp_PlayerInterface>()) {
		ICpp_PlayerInterface::Execute_UpdatePlayerAttributePoints(GetAvatarActor(), -1);
	}
}

void UCpp_ASC_Master::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
void UCpp_ASC_Master::ClientAbilityStatusChanged_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel) {
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}
void UCpp_ASC_Master::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag) {
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PreviousSlotTag);
}

void UCpp_ASC_Master::OnRep_ActivateAbilities() {
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven) {
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}
