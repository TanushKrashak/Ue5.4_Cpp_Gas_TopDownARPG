// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cpp_DA_LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()		

	UPROPERTY(EditDefaultsOnly)
	int32 ExperienceRequired = 100;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointsGained = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointsGained = 1;
};


UCLASS()
class AURA_API UCpp_DA_LevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	
	/// <The 0th Index for this array will always be empty, as the first level is always 1>
	UPROPERTY(EditDefaultsOnly)
	TArray<FLevelUpInfo> LevelUpInfo;


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================

	int32 FindLevelForExperience(int32 Experience) const;

};
