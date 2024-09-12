// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Cpp_PC_Player.generated.h"

// Forward Declarations
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ICpp_InteractionInterface;
class UCpp_DA_InputConfig;
class UCpp_ASC_Master;
class USplineComponent;
class UCpp_WGTComp_DamageText;
class UNiagaraSystem;

UCLASS()
class AURA_API ACpp_PC_Player : public APlayerController
{
	GENERATED_BODY()

	public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_PC_Player();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable, Category = "Combat")
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, const bool bBlockedHit, 
						  const bool bIsCriticalHit);

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Input Mapping Context
	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	// Move Input Actions
	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	TObjectPtr<UInputAction> MoveAction;

	// Shift Input Actions
	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping")
	TObjectPtr<UCpp_DA_InputConfig> InputConfig;

	// Last and Current Actor, TScriptInterface is used instead of normal pointers to avoid memory leaks
	TScriptInterface<ICpp_InteractionInterface> LastActor;
	TScriptInterface<ICpp_InteractionInterface> CurrentActor;	

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCpp_ASC_Master> RPGAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> OnClickNiagaraSystem;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	void Move(const FInputActionValue& Value);
	
	void CursorTrace();

	// Ability Input Tag Callbacks
	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);
	void AbilityInputTagHeld(FGameplayTag Tag);

	UCpp_ASC_Master* GetASC();



private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Variables For Moving The Player on Mouse Click
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	float ShortPressThreshold = 0.5f;
	bool bIsAutoRunning = false;
	bool bIsTargeting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Auto Run")
	float AutoRunAcceptanceRadius = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "Auto Run")
	TObjectPtr<USplineComponent> SplineComponent;

	FHitResult CursorHitResult;

	bool bIsShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UCpp_WGTComp_DamageText> DamageTextWidgetCompClass;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	void AutoRun();

	void ShiftSPressed();
	void ShiftReleased();
};
