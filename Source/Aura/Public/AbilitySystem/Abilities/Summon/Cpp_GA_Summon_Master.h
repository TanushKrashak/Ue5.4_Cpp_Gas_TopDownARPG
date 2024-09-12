// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GameplayAbility_RPG.h"
#include "Cpp_GA_Summon_Master.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_Summon_Master : public UCpp_GameplayAbility_RPG
{
	GENERATED_BODY()
	
public:
	//==============================================================================================================
	// PROPERTIES & VARIABLES
	//==============================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumberOfSpawns = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> SpawnClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpreadAngle = 120.0f;

	//==============================================================================================================
	// FUNCTIONS
	//==============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomSpawnClass();
};
