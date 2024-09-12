// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Cpp_GA_ProjectileSpell.h"
#include "Actors/Projectiles/Cpp_Projectile_Master.h"
#include "Interfaces/Cpp_CombatInterface.h"

// Engine Includes




void UCpp_GA_ProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch, const float PitchOverride) {

	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
		return;

	// Get the Combat Interface
	ICpp_CombatInterface* CombatInterface = Cast<ICpp_CombatInterface>(GetAvatarActorFromActorInfo());

	if (!CombatInterface) {
		UE_LOG(LogTemp, Error, TEXT("Cpp_GA_ProjectileSpell::ActivateAbility: Combat Interface is not implemented!"));
		return;
	}
	// Get the Socket Location (Execute is required for BlueprintNativeEvent)
	const FVector SocketLocation = ICpp_CombatInterface::Execute_GetSocketLocation(
		GetAvatarActorFromActorInfo(), SocketTag);
	// Get the Rotation
	FRotator TargetRotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	// Override the Pitch if required
	TargetRotation.Pitch = bOverridePitch ? PitchOverride : TargetRotation.Pitch;

	// Setup the Spawn Transform
	FTransform SpawnTransform;	
	SpawnTransform.SetRotation(TargetRotation.Quaternion());
	SpawnTransform.SetLocation(SocketLocation);	

	// Spawn the Projectile, but defer the spawning
	ACpp_Projectile_Master* Projectile = GetWorld()->SpawnActorDeferred<ACpp_Projectile_Master>(
		ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningComponentFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// Set the Projectile's Damage Effect Params
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	/* CAN BE USED FOR MULTIPLE DAMAGE TYPES IN THE FUTURE
	// Do the damage based on the Damage Types Map
	for (auto& DmgType : DamageTypes) {
		const float ScaledDamage = DmgType.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DmgType.Key, ScaledDamage);
	}
	*/
	Projectile->FinishSpawning(SpawnTransform);
}