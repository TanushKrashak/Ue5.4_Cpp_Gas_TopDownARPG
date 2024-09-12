// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Character/Cpp_Character.h"
#include "Interfaces/Cpp_PlayerInterface.h"
#include "Cpp_Character_Player.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;


UCLASS()
class AURA_API ACpp_Character_Player : public ACpp_Character, public ICpp_PlayerInterface
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================



	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_Character_Player();

	// Initialize the Gameplay Ability System
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Inherited from ICpp_CombatInterface
	//===========================================================
	virtual int32 GetPlayerLevel_Implementation() const override;
	//===========================================================

	// Inherited from ICpp_PlayerInterface
	//===========================================================
	virtual void UpdateExperience_Implementation(int32 ExperienceToAdd) override;
	virtual void UpdatePlayerLevel_Implementation(int32 LevelsToAdd) override;
	virtual void UpdatePlayerAttributePoints_Implementation(int32 AttributePointsToAdd) override;
	virtual void UpdatePlayerSpellPoints_Implementation(int32 SpellPointsToAdd) override;

	virtual int32 GetExperience_Implementation() const override;
	virtual int32 GetAttributPointsRewardForLevelUp_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsRewardForLevelUp_Implementation(int32 Level) const override;
	virtual int32 FindLevelForExperience_Implementation(int32 Experience) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	UFUNCTION(Category = "Player|Experience")
	virtual void LevelUp_Implementation() override;
	//===========================================================


protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Experience")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void InitAbilityActorInfo() override;

	virtual void OnRep_Shocked() override;
	virtual void OnRep_Burnt() override;

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(NetMulticast, Reliable)
	void MC_LevelUpNiagara() const;



};
