// No Copyright!

// Game Includes
#include "UI/Cpp_WidgetController_HUD.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Core/Cpp_PS_Player.h"
#include "AbilitySystem/DataAssets/Cpp_DA_LevelUpInfo.h"
#include "AbilitySystem/DataAssets/Cpp_DA_AbilityInfo.h"
#include "Cpp_AllGameplayTags.h"


void UCpp_WidgetController_HUD::BroadcastInitialValues() {	
	OnHealthChanged.Broadcast(GetCastedASet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetCastedASet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetCastedASet()->GetMana());
	OnMaxManaChanged.Broadcast(GetCastedASet()->GetMaxMana());
}

void UCpp_WidgetController_HUD::BindCallbacksToDependancies() {
	
	#pragma region Health and Mana Bindings
	// Health Binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCastedASet()->GetHealthAttribute()).
		AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			}				
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCastedASet()->GetMaxHealthAttribute()).
		AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// Mana Binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCastedASet()->GetManaAttribute()).
		AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetCastedASet()->GetMaxManaAttribute()).
		AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	#pragma endregion
	

	if (GetCastedASC()) {
		// Bind to Ability Equipped	
		GetCastedASC()->AbilityEquippedDelegate.AddUObject(this, &UCpp_WidgetController_HUD::OnAbilityEquipped);

		if (GetCastedASC()->bStartupAbilitiesGiven) {
			BroadcastAbilityInfo();
		}
		else {
			// Bind to when Abilities are given to the Avatar
			GetCastedASC()->AbilitiesGivenDelegate.AddUObject(this, &UCpp_WidgetController_HUD::BroadcastAbilityInfo);
		}

		// Gameplay Effect Tags Binding and creation of a anonymous lambda function
		GetCastedASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags) {
			for (const FGameplayTag& Tag : AssetTags) {
				// Create temporary GameplayTag of "Message" to compare with the AssetTags
				auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				// "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				if (Tag.MatchesTag(MessageTag)) {
					const FUIWidgetRowData* Row = GetDataTableRowByTag<FUIWidgetRowData>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
		);
	}

	// Get PS_Player and bind to Stat Changes	
	GetCastedPS()->OnExperienceChanged.AddUObject(this, &UCpp_WidgetController_HUD::OnExperienceChangedCallback);	
	// Bind to Level Changed
	GetCastedPS()->OnLevelChanged.AddLambda(
		[this](int32 NewLevel) {
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);	
	
}

void UCpp_WidgetController_HUD::OnExperienceChangedCallback(int32 NewExperience) {		
	// Get the LevelUpInfo from the Player State
	const UCpp_DA_LevelUpInfo* LevelUpInfo = GetCastedPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("LevelUpInfo is not set in the Player State Blueprint!"));

	const int32 Level = LevelUpInfo->FindLevelForExperience(NewExperience);
	// not the actual max Level, but the max Level in the LevelUpInfo, that is, till when the data is available
	const int32 MaxLevel = LevelUpInfo->LevelUpInfo.Num();

	if (Level <= MaxLevel && Level > 0) {
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInfo[Level].ExperienceRequired;
		const int32 PreviousLevelRequirement = Level > 1 ? LevelUpInfo->LevelUpInfo[Level - 1].ExperienceRequired : 0;

		const int32 DeltaRequirement = LevelUpRequirement - PreviousLevelRequirement;
		const int32 ExperienceForThisLevel = NewExperience - PreviousLevelRequirement;

		const float Progress = static_cast<float>(ExperienceForThisLevel) / static_cast<float>(DeltaRequirement);
		OnExperienceChanged.Broadcast(Progress);
	}
}

void UCpp_WidgetController_HUD::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const {

	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();

	// Clear out the previous slot
	FAbilityInfo_RPG LastSlotInfo = FAbilityInfo_RPG();
	LastSlotInfo.StatusTag = AllTags.Ability_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = AllTags.Ability_None;

	// Broadcast empty info if we had a previous slot (only if equipping an already equipped ability)
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill in the new slot
	FAbilityInfo_RPG Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = AllTags.Ability_Status_Equipped;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
