// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Cpp_GA_RPG_Damage.h"
#include "Cpp_GA_Damage_BeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_GA_Damage_BeamSpell : public UCpp_GA_RPG_Damage
{
	GENERATED_BODY()
	
public:

	//============================================================================================================== =
	// PROPERTIES & VARIABLES
	//===============================================================================================================


	//============================================================================================================== =
	// FUNCTIONS
	//===============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreMouseData(const FHitResult& Result);

	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable, Category = "Beam")
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Beam")
	void FindAndStoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void AdditionalTargetDied(AActor* DeadActor);

protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(BlueprintReadWrite, Category = "Beam")		
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxAdditionalTargets = 5;
};	
