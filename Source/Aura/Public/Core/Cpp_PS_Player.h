// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Cpp_PS_Player.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UAttributeSet;
class UCpp_DA_LevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* Stat Value */);


UCLASS()
class AURA_API ACpp_PS_Player : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	
	// Player Stats Delegates
	FOnPlayerStatChanged OnExperienceChanged;
	FOnPlayerStatChanged OnLevelChanged;
	FOnPlayerStatChanged OnAttributePointsChanged;
	FOnPlayerStatChanged OnSpellPointsChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Level Up")
	TObjectPtr<UCpp_DA_LevelUpInfo> LevelUpInfo;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_PS_Player();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Inherited from IAbilitySystemInterface
	//==========================================================
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//========================================================== 
	
	UAttributeSet* GetAttributeSet() const;

	// Level
	FORCEINLINE int32 GetPlayerLevel() const;
	void SetLevel(int32 NewLevel);
	void UpdateLevel(int32 LevelChange);

	// Experience
	FORCEINLINE int32 GetExperience() const;
	void SetExperience(int32 NewExperience);
	void UpdateExperience(int32 ExperienceChange);

	// Attribute Points	
	FORCEINLINE int32 GetAttributePoints() const;
	void SetAttributePoints(int32 NewAttributePoints);
	void UpdateAttributePoints(int32 AttributePointsChange);

	// Spell Points
	FORCEINLINE int32 GetSpellPoints() const;
	void SetSpellPoints(int32 NewSpellPoints);
	void UpdateSpellPoints(int32 SpellPointsChange);

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// Player Stats
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Experience, Category = "Level")
	int32 Experience = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints, Category = "Points")
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints, Category = "Points")
	int32 SpellPoints = 0;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	
	// Rep Notifies
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_Experience(int32 OldExperience);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	
};
