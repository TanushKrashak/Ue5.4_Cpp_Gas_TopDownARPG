// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GA_ProjectileSpell.h"
#include "Cpp_GA_Projectile_Firebolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_Projectile_Firebolt : public UCpp_GA_ProjectileSpell
{
	GENERATED_BODY()

public:
	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	virtual FString GetDescriptionForLevel(int32 Level, bool bNextLevel = false) const override;
			
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, 
						 const FGameplayTag& SocketTag, 
						 const bool bOverridePitch = false,
						 const float PitchOverride = 0.0f,
						 const AActor* HomingTarget = nullptr);

protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Firebolt")
	float ProjectileSpread = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Firebolt")
	bool bHomingProjectile = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Firebolt")
	float HomingAccelerationMin = 1600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Firebolt")
	float HomingAccelerationMax = 3200.0f;

};
