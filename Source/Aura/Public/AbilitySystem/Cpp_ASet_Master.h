// No Copyright!>

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Cpp_ASet_Master.generated.h"

struct FGameplayEffectModCallbackData;

// Macro to create the getter, setter and initter for the attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT()
struct FEffectProperties {
	GENERATED_BODY()
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// typedef is specific to the FGameplayAttribute() signature, but TSaticFunctionPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFunctionPtr;
template<class T>
using TStaticFunctionPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class AURA_API UCpp_ASet_Master : public UAttributeSet
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Stores All Gameplay Tags and their corresponding Attributes via Function Pointers To Delegate Instances
	TMap<FGameplayTag, TStaticFunctionPtr<FGameplayAttribute()>> TagToAttributes;

	
	// Vital Attributes
	//===========================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Main Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="Main Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Mana);
	//==========================================================


	// Meta Attributes
	//===========================================================
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, IncomingDamage);
	
	// Experience for when killing enemies
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingExperience;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, IncomingExperience);
	//===========================================================


	// Primary Attributes
	//===========================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Vigor);
	//===========================================================


	// Secondary Attributes 
	//==========================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, ManaRegeneration);
	//========================================================== 


	// Damage Resistance Attributes
	//==========================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Damage Resistance")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Damage Resistance")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Damage Resistance")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Damage Resistance")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCpp_ASet_Master, PhysicalResistance);
	//==========================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UCpp_ASet_Master();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// This is called before any attribute change is made
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// This is called after any attribute change is made
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	
	// Vital Attributes RepNotify
	//==========================================================
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);
	//==========================================================
	
	
	// Primary Attributes RepNotify
	//==========================================================
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience);
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor);
	//==========================================================


	// Secondary Attributes RepNotify
	//==========================================================
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance);
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration);
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);
	//========================================================== 


	// Damage Resistance Attributes RepNotify
	//==========================================================
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance);
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance);
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance);
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance);
	//==========================================================

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	bool bTopOffHealth = false;
	bool bTopOffMana = false;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProps) const;

	void ShowDamageText(const FEffectProperties& Props, float DamageAmount, const bool bBlockedHit,
						const bool bIsCriticalHit) const;

	void SendExperienceOnDeath(const FEffectProperties& Props) const;
};
