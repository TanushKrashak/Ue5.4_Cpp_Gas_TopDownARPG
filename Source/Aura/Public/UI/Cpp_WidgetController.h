// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cpp_WidgetController.generated.h"

// Forward Declarations
class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;
class ACpp_PC_Player;
class ACpp_PS_Player;
class UCpp_ASC_Master;
class UCpp_ASet_Master;
class UCpp_DA_AbilityInfo;

// Player State Stat Change Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, newValue);

// Ability Info Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAbilityInfo_RPG&, AbilityInfo);


USTRUCT(BlueprintType)
struct FWidgetControllerParams {
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* ASet) :
		PlayerController(PC), 
		PlayerState(PS),
		AbilitySystemComponent(ASC),
		AttributeSet(ASet) 
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


UCLASS()
class AURA_API UCpp_WidgetController : public UObject
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Ability Info Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS | Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependancies();

	void BroadcastAbilityInfo();

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================

	// Player Data
	UPROPERTY(BlueprintReadOnly, Category = "Player Data")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data")
	TObjectPtr<UAttributeSet> AttributeSet;

	// Casted
	UPROPERTY(BlueprintReadOnly, Category = "Player Data|Casted")
	TObjectPtr<ACpp_PC_Player> Casted_PC;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data|Casted")
	TObjectPtr<ACpp_PS_Player> Casted_PS;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data|Casted")
	TObjectPtr<UCpp_ASC_Master> Casted_ASC;
	UPROPERTY(BlueprintReadOnly, Category = "Player Data|Casted")
	TObjectPtr<UCpp_ASet_Master> Casted_ASet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS | Abilities")
	TObjectPtr<UCpp_DA_AbilityInfo> AbilityInfo;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	
	// Getters For Casted Variables
	UFUNCTION(BlueprintCallable, Category = "Player Data|Casted")
	ACpp_PC_Player* GetCastedPC();
	UFUNCTION(BlueprintCallable, Category = "Player Data|Casted")
	ACpp_PS_Player* GetCastedPS();
	UFUNCTION(BlueprintCallable, Category = "Player Data|Casted")
	UCpp_ASC_Master* GetCastedASC();
	UFUNCTION(BlueprintCallable, Category = "Player Data|Casted")
	UCpp_ASet_Master* GetCastedASet();

};
