// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GA_RPG_Damage.h"
#include "Cpp_GA_ProjectileSpell.generated.h"

// Forward Declarations
class ACpp_Projectile_Master;
class UGameplayEffect;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_ProjectileSpell : public UCpp_GA_RPG_Damage
{
	GENERATED_BODY()

	
protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<ACpp_Projectile_Master> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	int32 MaxNumberOfProjectiles = 5;

	//===============================================================================================================
	// FUNCTIONS
	//==============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, 
						 const FGameplayTag& SocketTag, 
						 const bool bOverridePitch = false,
						 const float PitchOverride = 0.0f);

};
