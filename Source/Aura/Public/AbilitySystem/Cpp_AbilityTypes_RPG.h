#pragma once

#include "GameplayEffectTypes.h"
#include "Cpp_AbilityTypes_RPG.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UGameplayEffect;


USTRUCT(BlueprintType)
struct FDamageEffectParams {
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.0f;	 

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 500.0f;
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackImpulseMagnitude = 50.0f;
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackImpulseChance = 0.0f;

};

USTRUCT(BlueprintType)
struct FGameplayEffectContext_RPG : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================


	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	// Returns the actual struct used for Serialization
	virtual UScriptStruct* GetScriptStruct() const override;

	// Creates a copy of the context, used to duplicate for later modification
	virtual FGameplayEffectContext* Duplicate() const override;

	// Custom Serialization
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	// Blocked Hit
	bool IsBlockedHit() const;
	void SetBlockedHit(const bool bNewBlock);

	// Critical Hit
	bool IsCriticalHit() const;
	void SetCriticalHit(const bool bNewCrit);

	// Debuff Getters
	bool IsSuccessfulDebuff() const;
	float GetDebuffDamage() const;
	float GetDebuffFrequency() const;
	float GetDebuffDuration() const;
	TSharedPtr<FGameplayTag> GetDamageType() const;
	// Debuff Setters
	void SetIsSuccessfulDebuff(const bool bNewDebuff);
	void SetDebuffDamage(const float NewDebuffDamage);
	void SetDebuffFrequency(const float NewDebuffFrequency);
	void SetDebuffDuration(const float NewDebuffDuration);	
	void SetDamageType(const TSharedPtr<FGameplayTag> InDamageType);

	// Death Impulse 
	FVector GetDeathImpulse() const;
	void SetDeathImpulse(const FVector& NewDeathImpulse);

	// Knockback Impulse
	FVector GetKnockbackImpulse() const;
	void SetKnockbackImpulse(const FVector& NewKnockbackImpulse);

protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCritialHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	UPROPERTY()
	float DebuffDamage = 0.0f;
	UPROPERTY()
	float DebuffFrequency = 0.0f;
	UPROPERTY()
	float DebuffDuration = 0.0f;


	TSharedPtr<FGameplayTag> DamageType;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY()
	FVector KnockbackImpulse = FVector::ZeroVector;
};

// The point of the struct is to define a custom gameplay effect context for RPG abilities.
template<>
struct TStructOpsTypeTraits<FGameplayEffectContext_RPG> : public TStructOpsTypeTraitsBase2<FGameplayEffectContext_RPG> {
	enum {
		WithNetSerializer = true,
		WithCopy = true
	};
};