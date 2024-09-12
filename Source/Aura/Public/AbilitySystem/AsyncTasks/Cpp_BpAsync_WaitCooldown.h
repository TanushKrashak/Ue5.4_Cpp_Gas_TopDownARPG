// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "Cpp_BpAsync_WaitCooldown.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);


/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UCpp_BpAsync_WaitCooldown : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	// This is the delegate that will be called when the cooldown starts
	UPROPERTY(BlueprintAssignable, Category = "Cooldown")
	FCooldownChangeSignature CooldownStart;
	// This is the delegate that will be called when the cooldown ends
	UPROPERTY(BlueprintAssignable, Category = "Cooldown")
	FCooldownChangeSignature CooldownEnd;



	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Cooldown", meta = (BlueprintInternalUseOnly = "true"))
	static UCpp_BpAsync_WaitCooldown* WaitForCooldown(UAbilitySystemComponent* AbilitySystemComponent, 
													  const FGameplayTag& InCooldownTags);

	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	void EndTask();



protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, 
							 const FGameplayEffectSpec& SpecApplied, 
							 FActiveGameplayEffectHandle ActiveHandle);
};
