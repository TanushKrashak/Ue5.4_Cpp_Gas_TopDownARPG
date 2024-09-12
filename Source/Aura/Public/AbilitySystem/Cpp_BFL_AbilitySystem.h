// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/DataAssets/Cpp_DA_CharacterClassInfo.h"
#include "Cpp_BFL_AbilitySystem.generated.h"

// Forward Declarations
class UCpp_WidgetController_HUD;
class UCpp_WidgetController_Attributes;
class ACpp_HUD_RPG;
class UCpp_WidgetController_Abilities;
class UCpp_WidgetController;
struct FWidgetControllerParams;


UCLASS()
class AURA_API UCpp_BFL_AbilitySystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//=================================================================================================================
	// FUNCTIONS
	//=================================================================================================================
	
	// Widgets
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Widget Controller", meta=(DefaultToSelf = "WorldContextObject"))
	static UCpp_WidgetController_HUD* GetHUDWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Widget Controller", meta = (DefaultToSelf = "WorldContextObject"))
	static UCpp_WidgetController_Attributes* GetAttributeWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Widget Controller", meta = (DefaultToSelf = "WorldContextObject"))
	static UCpp_WidgetController_Abilities* GetAbilitiesWidgetController(const UObject* WorldContextObject);


	// Character Class Initialization
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Character Class Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, 
											const ECharacterClass CharacterClass, 
											const float Level, 
											UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Character Class Defaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, 
									 UAbilitySystemComponent* ASC, 
									 const ECharacterClass CharacterClass);

	// Gamemode Getters
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Character Class Defaults")
	static UCpp_DA_CharacterClassInfo* GetCharacterClassInfoDA(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Ability Information")
	static UCpp_DA_AbilityInfo* GetAbilityInfoDA(const UObject* WorldContextObject);


	// AbilityTypes_RPG Networking Functions
	//==========================================================
	// Block
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static bool GetIsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
								const bool bIsBlockedHit);

	// Critical Hit
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
								const bool bIsCriticalHit);

	// Debuff Getters	
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static bool GetIsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	// Debuff Setters
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
									  const bool bIsSuccessfulDebuff);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
								const float DebuffDamage);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
								   const float DebuffFrequency);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
								  const float DebuffDuration);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
							  const FGameplayTag& InDamageType);

	// Death Impulse
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static FVector GetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle);	
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
								const FVector& NewDeathImpulse);

	// Knockback Impulse
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Effects")
	static FVector GetKnockbackImpulse(FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static void SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, 
									const FVector& NewKnockbackImpulse);
	//==========================================================

	// Combat
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Mechanics")
	static void GetAlivePlayersWithinRadius(const UObject* WorldContextObject, 
											TArray<AActor*>& OutActors,
											UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
											const FVector& Origin, 
											const float Radius);
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Mechanics")
	static void GetClosestTarget(int32 MaxNumberOfTargets,
								 const TArray<AActor*>& Actors,
								 TArray<AActor*>& OutClosestTargets,
								 FVector Origin);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Mechanics")
	static bool IsNotFriend(const AActor* ActorA, const AActor* ActorB);

	// Spread Mechanic Helpers
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Mechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumberOfRotaters);
	UFUNCTION(BlueprintPure, Category = "Cpp Ability System Library | Gameplay Mechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumberOfVectors);

	// Experience
	static int32 GetExpForKillOnClassAndLevel(const UObject* WorldContextObject, 
													const ECharacterClass CharacterClass, 
													const float Level);

	// Damage 
	UFUNCTION(BlueprintCallable, Category = "Cpp Ability System Library | Gameplay Effects")
	static FGameplayEffectContextHandle ApplyDamageEffectWithDamageParams(const FDamageEffectParams& Params);

private:
	//=================================================================================================================
	// FUNCTIONS
	//=================================================================================================================
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, ACpp_HUD_RPG*& OutHud);
};


