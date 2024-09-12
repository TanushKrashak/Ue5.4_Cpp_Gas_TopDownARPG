// No Copyright!

// Game Includes
#include "Actors/Projectiles/Cpp_Projectile_Master.h"
#include "Aura/Aura.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"

// Engine Includes
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ACpp_Projectile_Master::ACpp_Projectile_Master()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup Replication
	bReplicates = true;	

	// Setup SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);

	// Setup ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 550.0f;
	ProjectileMovementComponent->MaxSpeed = 550.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void ACpp_Projectile_Master::Destroyed() {
	if (!bHit && !HasAuthority()) {
		OnHitEffects();
	}
	if (LoopingSoundEffect) {
		LoopingSoundEffect->Stop();
		LoopingSoundEffect->DestroyComponent();
	}
	Super::Destroyed();
}

void ACpp_Projectile_Master::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ProjectileLifeSpan);

	// Setup Replication
	SetReplicateMovement(true);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACpp_Projectile_Master::OnSphereOverlap);
	
	if (LoopingSound) {
		LoopingSoundEffect = UGameplayStatics::SpawnSoundAttached(LoopingSound, RootComponent);
	}

}

void ACpp_Projectile_Master::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (DamageEffectParams.SourceASC == nullptr) {
		return;
	}
	const AActor* SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	// Do nothing if the projectile is hitting the owner
	if (SourceAvatarActor == OtherActor) {
		return;
	}
			
	// Do nothing if the projectile is hitting a friend
	if (!UCpp_BFL_AbilitySystem::IsNotFriend(SourceAvatarActor, OtherActor)) {
		return;
	}
	// Spawn Impact Effects if it hasn't already
	if (!bHit) {
		OnHitEffects();
		bHit = true;
	}
	if (HasAuthority()) {	
		// Apply Gameplay Effect on the Other Actor
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)) {
			// Death Impulse
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// Knockback Impulse
			const bool bShouldKnockback = FMath::RandRange(0.f, 100.f) < DamageEffectParams.KnockbackImpulseChance;
			if (bShouldKnockback) {
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackImpulse = KnockbackDirection * DamageEffectParams.KnockbackImpulseMagnitude;
				DamageEffectParams.KnockbackImpulse = KnockbackImpulse;
			}

			DamageEffectParams.TargetASC = TargetASC;
			UCpp_BFL_AbilitySystem::ApplyDamageEffectWithDamageParams(DamageEffectParams);
		}
		Destroy();	
	}	
	else {
		bHit = true;
	}
}

void ACpp_Projectile_Master::OnHitEffects() {
	// Spawn Impact Sound
	if (ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	// Spawn Impact Effect
	if (ImpactEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
}

