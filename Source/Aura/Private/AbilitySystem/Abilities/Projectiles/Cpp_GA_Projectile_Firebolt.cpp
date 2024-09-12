// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Projectiles/Cpp_GA_Projectile_Firebolt.h"
#include "Cpp_AllGameplayTags.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "Actors/Projectiles/Cpp_Projectile_Master.h"

// Engine Includes
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UCpp_GA_Projectile_Firebolt::GetDescriptionForLevel(int32 Level, bool bNextLevel /* = false*/) const {
	const FAllGameplayTags AllTags = FAllGameplayTags::Get();
	// Stats Required To use

	//const int32 Damage = GetDamageByDamageType(AllTags.Damage_Fire, Level); // CAN BE USED FOR MULTIPLE DAMAGE TYPES
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const auto NextLevelString = bNextLevel ? TEXT(" At Next Level!") : TEXT("!");
	const int NumOfProjectiles = FMath::Min(Level, MaxNumberOfProjectiles);
	const int ManaCost = abs(GetManaCostAtLevel(Level));
	const float Cooldown = GetCooldownAtLevel(Level);

	if (Level == 1) {
		return FString::Printf(TEXT(
			// Title
			"<Title>FireBall!</> \n\n"

			// Stats
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%d</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Launches a ball of fire, exploding on impact and dealing</>"
			"<Damage> % d</> "
			"<Default>Fire Damage with a chance to burn</>"), 

			// Args
			Level, ManaCost, Cooldown, ScaledDamage);
	}

	else {
		return FString::Printf(TEXT(
			// Title
			"<Title>FireBall%s</> \n\n"

			// Stats
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%d</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Launches %d balls of fire, exploding on impact and dealing</>"
			"<Damage> % d</> "
			"<Default>Fire Damage with a chance to burn</>"),

			// Args
			NextLevelString, Level, ManaCost, Cooldown, NumOfProjectiles, ScaledDamage);
	}
}

void UCpp_GA_Projectile_Firebolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch /*= false*/, const float PitchOverride /*= 0.0f*/, const AActor* HomingTarget /*= nullptr*/) {

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

	const FVector TargetDirection = TargetRotation.Vector();
	
	const int32 NumOfProjectiles = FMath::Min(GetAbilityLevel(), MaxNumberOfProjectiles);
	//const int32 NumOfProjectiles = 5;

	TArray<FRotator> SpreadDirections = UCpp_BFL_AbilitySystem::EvenlySpacedRotators(TargetDirection, FVector::UpVector, ProjectileSpread, NumOfProjectiles);

	for (const FRotator& Rot : SpreadDirections) {
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		ACpp_Projectile_Master* Projectile = GetWorld()->SpawnActorDeferred<ACpp_Projectile_Master>(
			ProjectileClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// Set the Homing Target
		if (bHomingProjectile) {
			if (HomingTarget && HomingTarget->Implements<UCpp_CombatInterface>()) {
				Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
			}
			else {
				Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
			}
			Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->ProjectileMovementComponent->bIsHomingProjectile = bHomingProjectile;
		}

		Projectile->FinishSpawning(SpawnTransform);
	}


	// Forward Arrow
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + TargetRotation.Vector() * 100.f, 5, FLinearColor::White, 50.0f, 1.0f);
	//
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 5, FLinearColor::Green, 50.0f, 1.0f);
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 5, FLinearColor::Red, 50.0f, 1.0f);
}
