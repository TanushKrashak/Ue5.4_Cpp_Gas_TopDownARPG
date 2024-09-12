// No Copyright!

// Game Includes
#include "Actors/Cpp_EffectActor.h"

// Engine Includes
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


// Sets default values
ACpp_EffectActor::ACpp_EffectActor()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
}


void ACpp_EffectActor::BeginPlay()
{
	Super::BeginPlay();
		
}

void ACpp_EffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) {
	
	// If Enemy But Not Apply To Enemies, return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) {
		return;
	}

	// Check if the target actor implements the AbilitySystemInterface
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
		return;

	// Check if the GameplayEffectClass is valid
	check(GameplayEffectClass);
	// Apply the GameplayEffect by creating appropriate context and spec
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());		

	// Check if the effect is infinite
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	//  Destroy On Effect Application, Only if instant effect
	if (bDestroyOnEffectApplication && !bIsInfinite) {
		Destroy();
	}
}

void ACpp_EffectActor::OnOverlap(AActor* TargetActor) {
	// If Enemy But Not Apply To Enemies, return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) {
		return;
	}

	// Apply Instant On Overlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	// Apply Duration On Overlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	// Apply Infinite On Overlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}
void ACpp_EffectActor::OnEndOverlap(AActor* TargetActor) {
	// If Enemy But Not Apply To Enemies, return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) {
		return;
	}

	// Apply Instant On End Overlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	// Apply Duration On End Overlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	// Apply Infinite On End Overlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	// Remove Infinite On End Overlap
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		// Check if the target actor implements the AbilitySystemInterface
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto& EffectPair : ActiveEffectHandles) {
			if (EffectPair.Value == TargetASC) {
				TargetASC->RemoveActiveGameplayEffect(EffectPair.Key, 1);
				HandlesToRemove.Add(EffectPair.Key);
			}
		}
		for (auto& Handle : HandlesToRemove) {	
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

