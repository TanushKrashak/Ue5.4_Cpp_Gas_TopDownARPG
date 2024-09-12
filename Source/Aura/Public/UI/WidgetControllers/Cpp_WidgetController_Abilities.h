// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UI/Cpp_WidgetController.h"
#include "GameplayTagContainer.h"
#include "Cpp_AllGameplayTags.h"
#include "Cpp_WidgetController_Abilities.generated.h"

// Delegate for when an ability is selected
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAbilitySelectedSignature, bool, bSpendPoints, bool, bEquip, FString, DescriptionString, FString, NextLevelDescriptionString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityReassignedSignature, const FGameplayTag&, AbilityType);


struct FSelectedAbility {
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	FGameplayTag StatusTag = FGameplayTag::EmptyTag;
};


UCLASS(BlueprintType, Blueprintable)
class AURA_API UCpp_WidgetController_Abilities : public UCpp_WidgetController
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(BlueprintAssignable, Category = "GAS | Player State | Stats")
	FOnPlayerStatChangedSignature OnSpellPointsChanged;	

	UPROPERTY(BlueprintAssignable, Category = "GAS | Abilities")
	FOnAbilitySelectedSignature OnAbilitySelectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | `Abilities")
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Abilities")
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Abilities")
	FAbilityReassignedSignature AbilityReassignedDelegate;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BindCallbacksToDependancies() override;
	virtual void BroadcastInitialValues() override;
	
	/// This function is called when an ability globe is selected In the Spell Menu		
	UFUNCTION(BlueprintCallable, Category = "GAS | Abilities")
	void OnAbilitySelected(const FGameplayTag& AbilityTag);
	/// This function is called when an ability globe is deselected In the Spell Menu
	UFUNCTION(BlueprintCallable, Category = "GAS | Abilities")
	void DeselectAbility();

	/// This function is called when the spend points button is pressed in the Spell Menu
	UFUNCTION(BlueprintCallable, Category = "GAS | Abilities")
	void SpendPointsButtonPressed();
	/// This function is called when the equip button is pressed in the Spell Menu
	UFUNCTION(BlueprintCallable, Category = "GAS | Abilities")
	void EquipButtonPressed();
	/// This function is called when any of the ability glboes are pressed in the Spell Menu
	UFUNCTION(BlueprintCallable, Category = "GAS | Abilities")
	void AbilityGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	FSelectedAbility SelectedAbility = {FAllGameplayTags::Get().Ability_None, FAllGameplayTags::Get().Ability_Status_Locked};

	int32 CurrentSpellPoints = 0;

	bool bWaitingForEquipSelection = false;

	FGameplayTag SelectedSlot;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	static void ShouldEnableButtons(const FGameplayTag& AbilityTag, int32 SpellPoints, bool& bOutSpendPoints, bool& bOutEquip);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag);
};
