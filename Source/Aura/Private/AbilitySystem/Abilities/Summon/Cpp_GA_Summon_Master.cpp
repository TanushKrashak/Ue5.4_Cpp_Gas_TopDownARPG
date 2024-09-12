// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Summon/Cpp_GA_Summon_Master.h"

// Engine Includes
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

TArray<FVector> UCpp_GA_Summon_Master::GetSpawnLocations() {	
	// Get Fwd Direction + Current Location + Individual Spread Angle
	const FVector ForwardDirection = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpreadAngle / NumberOfSpawns;

	const FVector LeftOfSpread = ForwardDirection.RotateAngleAxis(-SpawnSpreadAngle / 2.0f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (uint8 i = 0; i < NumberOfSpawns; i++) {
		const FVector SpreadDirection = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const FVector SpawnLocation = Location + SpreadDirection * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		// Adjust Spawn Location to the ground
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Location + FVector(0,0,400.f), SpawnLocation + FVector(0,0,-400.f), ECollisionChannel::ECC_Visibility);

		SpawnLocations.Add(HitResult.bBlockingHit ? HitResult.ImpactPoint : SpawnLocation);
	}	
	return SpawnLocations;
}

TSubclassOf<APawn> UCpp_GA_Summon_Master::GetRandomSpawnClass() {
	const uint8 RandomIndex = FMath::RandRange(0, SpawnClasses.Num() - 1);
	return SpawnClasses[RandomIndex];
}
		