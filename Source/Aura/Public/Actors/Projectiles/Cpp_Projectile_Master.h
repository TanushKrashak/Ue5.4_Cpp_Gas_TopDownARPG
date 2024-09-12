// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"
#include "Cpp_Projectile_Master.generated.h"

// Forward Declarations
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;


UCLASS()
class AURA_API ACpp_Projectile_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_Projectile_Master();

	virtual void Destroyed() override;

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	


private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
		
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> LoopingSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundEffect;

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly, Category = "Params")
	float ProjectileLifeSpan = 10.0f;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintCallable)
	void OnHitEffects();
};
