// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UI/Cpp_WidgetController.h"
#include "GameplayTagContainer.h"
#include "Cpp_WidgetController_HUD.generated.h"

// Forward Declarations
struct FOnAttributeChangeData;
class UCpp_WGT_Master;
class UTexture2D;
class UCpp_DA_AbilityInfo;
class UCpp_DA_LevelUpInfo;
class UCpp_ASC_Master;
struct FAbilityInfo_RPG;

USTRUCT(BlueprintType)
struct FUIWidgetRowData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageGamplayTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCpp_WGT_Master> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};	


// Attribute Changed Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, newValue);

// Gameplay Tag Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRowData, Row);



UCLASS(BlueprintType, Blueprintable)
class AURA_API UCpp_WidgetController_HUD : public UCpp_WidgetController
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Health
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	// Mana
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;

	// Gameplay Tag Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS | Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	

	// Player State Stat Change Delegates
	UPROPERTY(BlueprintAssignable, Category = "GAS | Player State | Stats")
	FOnAttributeChangeSignature OnExperienceChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Player State | Stats")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependancies() override;

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;
	

	UFUNCTION()
	void OnExperienceChangedCallback(int32 NewExperience);

	UFUNCTION()
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
};

template<typename T>
T* UCpp_WidgetController_HUD::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const {
	// Find the row in the DataTable
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));	
}

