// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Character/Cpp_Character.h"
#include "Interfaces/Cpp_InteractionInterface.h"
#include "UI/Cpp_WidgetController_HUD.h"
#include "Cpp_Character_Enemy.generated.h"

// Forward Declarations
class UWidgetComponent;
class UBehaviorTree;
class ACpp_AiC_Master;

UCLASS()
class AURA_API ACpp_Character_Enemy : public ACpp_Character, public ICpp_InteractionInterface
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_Character_Enemy();
		
	virtual void BeginPlay() override;

	// Inherited from ICpp_InteractionInterface
	//===========================================================
	virtual void HighlightActor() override;	
	virtual void UnhighlightActor() override;

	virtual void SetCombatTarget_Implementation(AActor* Target) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	//===========================================================



	// Inherited from ICpp_CombatInterface
	//===========================================================
	virtual int32 GetPlayerLevel_Implementation() const override;
	virtual void Die(const FVector& DeathImpulse) override;
	//===========================================================

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);




protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties|Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties|Combat")
	float OnDeathLifeSpan = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties|Ai")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ACpp_AiC_Master> AiControllerRef;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
};
