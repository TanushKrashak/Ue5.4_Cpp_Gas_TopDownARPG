// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "Cpp_AbilitySystemGlobals_RPG.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_AbilitySystemGlobals_RPG : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
