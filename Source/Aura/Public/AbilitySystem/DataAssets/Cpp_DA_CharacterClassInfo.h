// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "Cpp_DA_CharacterClassInfo.generated.h"

// Forward Declarations
class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8 {
	Warrior,
	Mage,
	Archer
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	FScalableFloat ExperienceOnKill = FScalableFloat();
};

UCLASS()
class AURA_API UCpp_DA_CharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults| Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	FCharacterClassDefaultInfo GetCharacterClassDefaults(ECharacterClass CharacterClass) const;
};
