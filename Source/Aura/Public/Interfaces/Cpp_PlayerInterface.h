// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cpp_PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCpp_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICpp_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Updaters
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Experience")
	void UpdateExperience(int32 ExperienceToAdd);
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	void UpdatePlayerLevel(int32 LevelsToAdd);
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	void UpdatePlayerAttributePoints(int32 AttributePointsToAdd);
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	void UpdatePlayerSpellPoints(int32 SpellPointsToAdd);

	// Getters
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Experience")
	int32 GetExperience() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	int32 GetAttributPointsRewardForLevelUp(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	int32 GetSpellPointsRewardForLevelUp(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	int32 FindLevelForExperience(int32 Experience) const;
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	int32 GetAttributePoints() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Player Interface | Level")
	void LevelUp();
};
