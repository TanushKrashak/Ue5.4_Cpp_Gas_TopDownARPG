// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GameplayAbility_RPG.h"
#include "Cpp_GA_RPG_Passive.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_RPG_Passive : public UCpp_GameplayAbility_RPG
{
	GENERATED_BODY()
	


public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================




	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	void ReceiveDeactivate(const FGameplayTag& AbilityTag);




};
