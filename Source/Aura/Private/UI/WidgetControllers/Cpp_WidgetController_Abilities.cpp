// No Copyright!

// Game Includes
#include "UI/WidgetControllers/Cpp_WidgetController_Abilities.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "AbilitySystem/DataAssets/Cpp_DA_AbilityInfo.h"
#include "Core/Cpp_PS_Player.h"

void UCpp_WidgetController_Abilities::BindCallbacksToDependancies() {
	GetCastedASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewLevel) {
			
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag)) {
				SelectedAbility.StatusTag = StatusTag;
				// Broadcast the new status
				bool bSpendPoints = false;
				bool bEquip = false;
				// Determine if we should be able to spend points or equip or both
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bSpendPoints, bEquip);
				FString DescriptionString;
				FString NextLevelDescriptionString;
				GetCastedASC()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);

				OnAbilitySelectedDelegate.Broadcast(bSpendPoints, bEquip, DescriptionString, NextLevelDescriptionString);
			}

			if (AbilityInfo) {
				FAbilityInfo_RPG Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	// Bind to Ability Equipped
	GetCastedASC()->AbilityEquippedDelegate.AddUObject(this, &UCpp_WidgetController_Abilities::OnAbilityEquipped);

	// Bind to Spell Points Changed
	GetCastedPS()->OnSpellPointsChanged.AddLambda(
		[this](int32 NewSpellPoints) {
			OnSpellPointsChanged.Broadcast(NewSpellPoints);
			CurrentSpellPoints = NewSpellPoints;
			// Broadcast the new status
			bool bSpendPoints = false;
			bool bEquip = false;
			// Determine if we should be able to spend points or equip or both
			ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bSpendPoints, bEquip);
			FString DescriptionString;
			FString NextLevelDescriptionString;
			GetCastedASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, DescriptionString, NextLevelDescriptionString);

			OnAbilitySelectedDelegate.Broadcast(bSpendPoints, bEquip, DescriptionString, NextLevelDescriptionString);	
		}
	);
}

void UCpp_WidgetController_Abilities::BroadcastInitialValues() {
	BroadcastAbilityInfo();

	// Broadcast the Spell Points
	OnSpellPointsChanged.Broadcast(GetCastedPS()->GetSpellPoints());
}

void UCpp_WidgetController_Abilities::OnAbilitySelected(const FGameplayTag& AbilityTag) {	
	// Stop waiting for equip if we were
	if (bWaitingForEquipSelection) {
	// Broadcast "wait for equip" to stop waiting
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();

	const int32 SpellPoints = GetCastedPS()->GetSpellPoints();

	// start with no status
	FGameplayTag AbilityStatus = FGameplayTag::EmptyTag;

	// Decider Bools
	const bool bValidTag = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(AllTags.Ability_None);
	const FGameplayAbilitySpec* AbilitySpec = GetCastedASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bValidTag || bTagNone || !bSpecValid) {
		AbilityStatus = AllTags.Ability_Status_Locked;
	}
	else {
		AbilityStatus = GetCastedASC()->GetAbilityStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;

	// Broadcast the new status
	bool bSpendPoints = false;
	bool bEquip = false;
	// Determine if we should be able to spend points or equip or both
	ShouldEnableButtons(AbilityStatus, SpellPoints, bSpendPoints, bEquip);
	FString DescriptionString;
	FString NextLevelDescriptionString;
	GetCastedASC()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);

	OnAbilitySelectedDelegate.Broadcast(bSpendPoints, bEquip, DescriptionString, NextLevelDescriptionString);
}
void UCpp_WidgetController_Abilities::DeselectAbility() {
	// Stop waiting for equip if we were
	if (bWaitingForEquipSelection) {
	// Broadcast "wait for equip" to stop waiting
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.AbilityTag = FAllGameplayTags::Get().Ability_None;
	SelectedAbility.StatusTag = FAllGameplayTags::Get().Ability_Status_Locked;

	OnAbilitySelectedDelegate.Broadcast(false, false, FString(), FString());
}

void UCpp_WidgetController_Abilities::SpendPointsButtonPressed() {
	GetCastedASC()->ServerSpendPoints(SelectedAbility.AbilityTag);
}
void UCpp_WidgetController_Abilities::EquipButtonPressed() {
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityTypeTag;
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetCastedASC()->GetStatusFromAbilityTag(SelectedAbility.AbilityTag);
	// If Equipped, store current slot
	if (SelectedStatus.MatchesTagExact(FAllGameplayTags::Get().Ability_Status_Equipped)) {
		SelectedSlot = GetCastedASC()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}
void UCpp_WidgetController_Abilities::AbilityGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType) {
	if (!bWaitingForEquipSelection) {
		return;
	}
	// check selected ability against slot's ability type (don't equip if they don't match)
	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityTypeTag;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) {
		return;
	}

	GetCastedASC()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void UCpp_WidgetController_Abilities::ShouldEnableButtons(const FGameplayTag& AbilityTag, int32 SpellPoints, bool& bOutSpendPoints, bool& bOutEquip) {
	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();
	
	// Equipped
	if (AbilityTag.MatchesTagExact(AllTags.Ability_Status_Equipped)) {
		bOutEquip = true;
		bOutSpendPoints = (SpellPoints > 0) ? true : false;
	}	
	// Eligible
	else if (AbilityTag.MatchesTagExact(AllTags.Ability_Status_Eligible)) {
		bOutEquip = false;
		bOutSpendPoints = (SpellPoints > 0) ? true : false;
	}
	// Unlocked
	else if (AbilityTag.MatchesTagExact(AllTags.Ability_Status_Unlocked)) {
		bOutEquip = true;
		bOutSpendPoints = (SpellPoints > 0) ? true : false;
	}
	// Locked
	else if (AbilityTag.MatchesTagExact(AllTags.Ability_Status_Locked)) {
		bOutEquip = false;
		bOutSpendPoints = false;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Ability Status not found for %s"), *AbilityTag.ToString());
	}
}

void UCpp_WidgetController_Abilities::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag) {
	bWaitingForEquipSelection = false;

	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();

	// Clear out the previous slot
	FAbilityInfo_RPG LastSlotInfo = FAbilityInfo_RPG();
	LastSlotInfo.StatusTag = AllTags.Ability_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlotTag;
	LastSlotInfo.AbilityTag = AllTags.Ability_None;

	// Broadcast empty info if we had a previous slot (only if equipping an already equipped ability)
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill in the new slot
	FAbilityInfo_RPG Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);

	// Broadcast "wait for equip" to stop waiting
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityTypeTag);

	// Broadcast to tell that specific ability was equipped
	AbilityReassignedDelegate.Broadcast(AbilityTag);
	DeselectAbility();
}
