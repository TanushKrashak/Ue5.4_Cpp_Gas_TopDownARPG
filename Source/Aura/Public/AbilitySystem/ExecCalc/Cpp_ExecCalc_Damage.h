// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Cpp_ExecCalc_Damage.generated.h"

UCLASS()
class AURA_API UCpp_ExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UCpp_ExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;


private:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

	void DetermineDebuff(const FGameplayEffectSpec& Spec, 
						 const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
						 FAggregatorEvaluateParameters EvaluationParameters, 
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDef) const;

};
