// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GameplayAbility_RPG.h"
#include "Interfaces/Cpp_CombatInterface.h"
#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"
#include "Cpp_GA_RPG_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_RPG_Damage : public UCpp_GameplayAbility_RPG
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "Damage")
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure, Category = "Damage")
	float GetDamageBasedOnLevel() const;

protected:
	//================================================================================================================
	// PROTECTED FUNCTIONS
	//================================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/* CAN BE USED FOR MULTIPLE DAMAGE TYPES IN THE FUTURE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffChance = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDamage = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float KnockbackImpulseMagnitude = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float KnockbackImpulseChance = 0.f;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintPure, Category = "Animation")
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages) const;

	/* CAN BE USED FOR MULTIPLE DAMAGE TYPES IN THE FUTURE
	float GetDamageByDamageType(const FGameplayTag& DamageType, int32 Level) const; */
};
