// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Cpp_ASC_Master.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
// used to broadcast that an ability has been given to the avatar
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
// used to iterate through all abilities
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
// used to broadcast that an ability status has changed
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const int32 /*AbilityLevel*/);
// used to broadcast that an ability has been equipped
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PreviousSlot*/)

DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);


UCLASS()
class AURA_API UCpp_ASC_Master : public UAbilitySystemComponent
{
	GENERATED_BODY()	

public:	
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	FEffectAssetTags EffectAssetTags; 

	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	FAbilityEquipped AbilityEquippedDelegate;

	FDeactivatePassiveAbility DeactivatePassiveAbilityDelegate;

	bool bStartupAbilitiesGiven = false;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	void AbilityActorInfoSet();

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	// Iterates through all abilities and executes the delegate
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	void UpdateAbilityStatuses(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerSpendPoints(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);

	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	void ClearSlot(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilitiesWithSlot(const FGameplayTag& SlotTag);
	static bool AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag);



protected:
	//================================================================================================================
	// PROTECTED FUNCTIONS
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	// Client RPCs
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	UFUNCTION(Client, Reliable)
	void ClientAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel);
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag,
							const FGameplayTag& PreviousSlotTag);

	virtual void OnRep_ActivateAbilities() override;


};
