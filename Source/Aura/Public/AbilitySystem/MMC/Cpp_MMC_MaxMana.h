// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Cpp_MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_MMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	//================================================================================================================
	// PROPERTY & VARIABLES
	//================================================================================================================



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UCpp_MMC_MaxMana();


	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


protected:
	//================================================================================================================
	// PROPERTY & VARIABLES
	//================================================================================================================
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================


};
