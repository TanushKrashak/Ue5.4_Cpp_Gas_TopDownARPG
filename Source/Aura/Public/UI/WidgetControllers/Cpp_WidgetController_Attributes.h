// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UI/Cpp_WidgetController.h"
#include "GameplayTagContainer.h"
#include "Cpp_WidgetController_Attributes.generated.h"

// Forward Declarations
struct FAttributeInfo_RPG;
class UCpp_DA_AttributeInfo;
struct FGameplayAttribute;
struct FGameplayTag;

// Attribute Changed Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAttributeInfo_RPG&, Info);


UCLASS(BlueprintType, Blueprintable)
class AURA_API UCpp_WidgetController_Attributes : public UCpp_WidgetController
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FAttributeInfoSignature OnAttributeInfoChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Player State | Stats")
	FOnPlayerStatChangedSignature OnAttributePointsChanged;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BindCallbacksToDependancies() override;
	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable, Category = "GAS | Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Attributes")
	TObjectPtr<UCpp_DA_AttributeInfo> AttributeInfo;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;


};
