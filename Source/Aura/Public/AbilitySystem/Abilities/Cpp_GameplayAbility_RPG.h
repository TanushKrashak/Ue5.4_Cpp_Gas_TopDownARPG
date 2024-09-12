// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Cpp_GameplayAbility_RPG.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GameplayAbility_RPG : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//===============================================================================================================
	// Properties & Variables
	//===============================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag StartupInputTag;

	//===============================================================================================================
	// Functions
	//===============================================================================================================
	virtual FString GetDescriptionForLevel(int32 Level, bool bNextLevel = false) const;
	static FString GetLockedDescription(int32 Level);

protected:
	//===============================================================================================================
	// Functions
	//===============================================================================================================
	// Getters For Stats
	int32 GetManaCostAtLevel(int32 Level = 1) const;
	float GetCooldownAtLevel(int32 Level = 1) const;
};
