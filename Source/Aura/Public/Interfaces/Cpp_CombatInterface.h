// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/DataAssets/Cpp_DA_CharacterClassInfo.h"
#include "Cpp_CombatInterface.generated.h"

// Forward Declarations
class UAnimMontage;
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAscRegistred, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);


USTRUCT(BlueprintType)
struct FTaggedMontage {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Montage"))
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="CombatSocket"))
	FGameplayTag SocketTag;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCpp_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICpp_CombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	int32 GetPlayerLevel() const;
	
	// Combat Functions
	virtual void Die(const FVector& DeathImpulse) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsDead() const;
	virtual FOnDeath& GetOnDeathDelegate() = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AActor* GetAvatar();		
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	ECharacterClass GetCharacterClass() const;	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	USkeletalMeshComponent* GetWeapon() const;


	// Summons Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int32 GetCurrentSummonsCount() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void UpdateSummonsCount(const int32 Amount = 1);


	// Combat Montage Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Montage")
	FVector GetSocketLocation(const FGameplayTag& MontageTag);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat|Montage")
	void SetFacingTargetLocation(const FVector& TargetLocation);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Montage")
	UAnimMontage* GetHitReactMontage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Montage")
	TArray<FTaggedMontage> GetAttackMontages();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Montage")
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag MontageTag) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Montage")
	UNiagaraSystem* GetHitReactEffect() const;


	// Ability System Functions
	virtual FOnAscRegistred GetOnAscRegistredDelegate() = 0;

	// Attack Ability Related Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool GetIsBeingShocked() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetIsBeingShocked(bool bInLoop);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat|Ability")
	void SetInElectrocuteAttackLoop(const bool bInLoop);
};
