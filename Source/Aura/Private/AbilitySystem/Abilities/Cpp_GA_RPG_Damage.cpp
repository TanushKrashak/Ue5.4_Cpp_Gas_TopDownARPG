// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Cpp_GA_RPG_Damage.h"

// Engine Includes
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCpp_GA_RPG_Damage::CauseDamage(AActor* TargetActor) {
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	/* CAN BE USED FOR MULTIPLE DAMAGE TYPES IN THE FUTURE
	for (const auto& Elem : DamageTypes) {
		float ScaledDamage = Elem.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Elem.Key, ScaledDamage);
	}
	*/

	float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	
}

FDamageEffectParams UCpp_GA_RPG_Damage::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor /*= nullptr*/) const {
	FDamageEffectParams Params;

	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();	
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageEffectClass = DamageEffectClass;
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.DamageType = DamageType;

	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DebuffDuration = DebuffDuration;

	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;
	Params.KnockbackImpulseChance = KnockbackImpulseChance;
	 
	if (IsValid(TargetActor)) {
		FRotator RotationToTarget = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		RotationToTarget.Pitch = 45.f;
		const FVector DirectionToTarget = RotationToTarget.Vector();
		Params.DeathImpulse = DirectionToTarget * DeathImpulseMagnitude;
		// Knockback Impulse
		const bool bShouldKnockback = FMath::RandRange(0.f, 100.f) < KnockbackImpulseChance;
		if (bShouldKnockback) {											
			Params.KnockbackImpulse = DirectionToTarget * KnockbackImpulseMagnitude;			
		}
	}
	return Params;
}

float UCpp_GA_RPG_Damage::GetDamageBasedOnLevel() const {
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UCpp_GA_RPG_Damage::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages) const {
	if (Montages.Num() > 0) {
		// Get a random montage from the array
		const int32 RandomIndex = FMath::RandRange(0, Montages.Num() - 1);
		return Montages[RandomIndex];
	}
	return FTaggedMontage();
}

/* CAN BE USED FOR MULTIPLE DAMAGE TYPES IN THE FUTURE
float UCpp_GA_RPG_Damage::GetDamageByDamageType(const FGameplayTag& DamageType, int32 Level) const {
	checkf(DamageTypes.Contains(DamageType), TEXT("Damage Type %s not found in DamageTypes map! GA_RPG_Damage"), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(Level);
}
*/
