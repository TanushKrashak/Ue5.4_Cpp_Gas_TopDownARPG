// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Cpp_AT_TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, 
											const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class AURA_API UCpp_AT_TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Target Data Under Mouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCpp_AT_TargetDataUnderMouse* CreateTargetDataUnderMouseTask(UGameplayAbility* OwningAbility);


private:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================



	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	virtual void Activate() override;

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
}; 
