// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "Cpp_NC_Debuff.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_NC_Debuff : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Debuff")
	FGameplayTag DebuffTag;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UCpp_NC_Debuff();

	virtual void BeginPlay() override;

	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
