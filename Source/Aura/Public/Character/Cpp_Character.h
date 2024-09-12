// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Cpp_CombatInterface.h"
#include "AbilitySystem/DataAssets/Cpp_DA_CharacterClassInfo.h"
#include "Cpp_Character.generated.h"

// Forward Declarations
class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UMaterialInstance;
class UNiagaraSystem;
class UCpp_NC_Debuff;

UCLASS(Abstract)
class AURA_API ACpp_Character : public ACharacter, public IAbilitySystemInterface, public ICpp_CombatInterface
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditAnywhere, Category = "Properties|Combat")
	TArray<FTaggedMontage> AttackMontages;


	FOnDeath OnDeathDelegate;
	FOnAscRegistred OnAscRegistredDelegate;

	UPROPERTY(ReplicatedUsing = OnRep_Shocked, BlueprintReadOnly, Category = "Combat")
	bool bIsShocked = false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsBeingShocked = false;
	UPROPERTY(ReplicatedUsing = OnRep_Burnt, BlueprintReadOnly, Category = "Combat")
	bool bIsBurnt = false;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_Character();

	virtual void BeginPlay() override;

		// Inherited from IAbilitySystemInterface
	//===========================================================
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//===========================================================


		// Inherited from ICpp_CombatInterface
	//===========================================================
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;	
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	virtual int32 GetCurrentSummonsCount_Implementation() const override;
	virtual void UpdateSummonsCount_Implementation(const int32 Amount);
	virtual FVector GetSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetAttackMontageByTag_Implementation(const FGameplayTag MontageTag) const;
	virtual UNiagaraSystem* GetHitReactEffect_Implementation() const override;
	virtual FOnAscRegistred GetOnAscRegistredDelegate() override;
	virtual bool GetIsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bNewValue) override;
	//===========================================================

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Combat")
	virtual void MC_HandleDeath(const FVector& DeathImpulse);
		
	UAttributeSet* GetAttributeSet() const;


protected:	
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// Socket Names
	UPROPERTY(EditDefaultsOnly, Category = "Properties|Combat| Sockets")
	FName BurnDebuffNiagaraSocket = "HeadSocket";
	UPROPERTY(EditDefaultsOnly, Category = "Properties|Combat| Sockets")
	FName ShockDebuffNiagaraSocket = "HeadSocket";
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName LeftHandAttackSocketName;
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName RightHandAttackSocketName;
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName LeftFootAttackSocketName;
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName RightFootAttackSocketName;
	UPROPERTY(EditAnywhere, Category = "Properties|Combat| Sockets")
	FName TailSocketName;

	// Ability System
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	// Default Attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Dissolve Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Combat| Materials")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	// Weapon Dissolve Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Combat| Materials")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// Niagara Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Combat| Niagara")
	UNiagaraSystem* HitReactNiagaraEffect;

	// Death
	bool bIsDead = false;
	// Sound Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Combat| Sound")
	USoundBase* DeathSound;

	// Summons
	uint8 CurrentSummonsCount = 0;
	
	// Debuff Niagara Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCpp_NC_Debuff> BurnDebuffComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCpp_NC_Debuff> ShockDebuffComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties|Defaults")
	float BaseWalkSpeed = 600.0f;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Setup Default Attributes
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> effect, float level) const;	
	
	virtual void InitAbilityActorInfo();
	virtual  void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	// Dissolve Effect
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// Stun Tag Changed Callback
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// Lifetime Replicated Properties
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Shocked();
	UFUNCTION()
	virtual void OnRep_Burnt();

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================

	// Startup Abilities
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Properties|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================


};
