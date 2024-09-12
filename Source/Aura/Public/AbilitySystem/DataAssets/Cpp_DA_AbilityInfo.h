// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Cpp_DA_AbilityInfo.generated.h"

// Forward Declarations
class UTexture2D;
class UMaterialInstance;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityInfo_RPG {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability.Type"))
	FGameplayTag AbilityTypeTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> AbilityIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInstance> AbilityBgMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UCpp_DA_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information")
	TArray<FAbilityInfo_RPG> AllAbilityInfo;


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	FAbilityInfo_RPG FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
