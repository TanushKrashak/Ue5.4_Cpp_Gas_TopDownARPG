// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Damage/Cpp_GA_Damage_BeamSpell.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"

// Engine Includes
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UCpp_GA_Damage_BeamSpell::StoreMouseData(const FHitResult& Result) {
	if (Result.bBlockingHit) {
		MouseHitLocation = Result.ImpactPoint;
		MouseHitActor = Result.GetActor();
	}
	else {
		// Cancel the ability
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

}

void UCpp_GA_Damage_BeamSpell::StoreOwnerVariables() {
	if (CurrentActorInfo) {
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UCpp_GA_Damage_BeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation) {
	if (OwnerCharacter->Implements<UCpp_CombatInterface>()) {
		if (USkeletalMeshComponent* Weapon = ICpp_CombatInterface::Execute_GetWeapon(OwnerCharacter)) {
			const FVector SocketLocation = Weapon->GetSocketLocation("Muzzle");

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);

			FHitResult HitResult;

			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
													SocketLocation,
													BeamTargetLocation,
													10.0f,
													ETraceTypeQuery::TraceTypeQuery1,
													false,
													ActorsToIgnore,
													EDrawDebugTrace::None,
													HitResult,
													true);
			if (HitResult.bBlockingHit) {
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}	
	if (ICpp_CombatInterface* CombatInterface = Cast<ICpp_CombatInterface>(MouseHitActor)) {
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UCpp_GA_Damage_BeamSpell::PrimaryTargetDied)) {
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UCpp_GA_Damage_BeamSpell::PrimaryTargetDied);
		}
	}
}

void UCpp_GA_Damage_BeamSpell::FindAndStoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets) {
	// Get All actors near the target location and only add the ones that are not the target actor and are an enemy
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(MouseHitActor.Get());
	ActorsToIgnore.AddUnique(OwnerCharacter);

	TArray<AActor*> OverlappingActors;

	UCpp_BFL_AbilitySystem::GetAlivePlayersWithinRadius(GetAvatarActorFromActorInfo(),
														OverlappingActors,
														ActorsToIgnore,
														MouseHitActor->GetActorLocation(),
														850.f);

	// Gets the min num between Max, Overlapped Actors as well as the level of the ability divided by 2
	uint8 NumOfAdditionalTargets = FMath::Min(MaxAdditionalTargets, OverlappingActors.Num());
	NumOfAdditionalTargets = FMath::Min(MaxAdditionalTargets, static_cast<int>(GetAbilityLevel()/2));
	//int NumOfAdditionalTargets = MaxAdditionalTargets;
	UCpp_BFL_AbilitySystem::GetClosestTarget(NumOfAdditionalTargets,
											OverlappingActors,
											OutAdditionalTargets,
											MouseHitActor->GetActorLocation());	

	for (AActor* AdditionalTarget : OutAdditionalTargets) {
		if (ICpp_CombatInterface* CombatInterface = Cast<ICpp_CombatInterface>(AdditionalTarget)) {
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UCpp_GA_Damage_BeamSpell::AdditionalTargetDied)) {
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UCpp_GA_Damage_BeamSpell::AdditionalTargetDied);
			}
		}
	}
}








