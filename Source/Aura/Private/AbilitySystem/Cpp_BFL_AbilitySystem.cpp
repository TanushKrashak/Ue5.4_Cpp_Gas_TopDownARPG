// No Copyright!

// Game Includes
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "UI/Cpp_WidgetController_HUD.h"
#include "UI/Cpp_HUD_RPG.h"
#include "Core/Cpp_PS_Player.h"
#include "Core/Cpp_GM_RPG.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "UI/Cpp_WidgetController.h"
#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"
#include "Interfaces/Cpp_CombatInterface.h"
#include "UI/WidgetControllers/Cpp_WidgetController_Abilities.h"
#include "Cpp_AllGameplayTags.h"

// Engine Includes
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UCpp_WidgetController_HUD* UCpp_BFL_AbilitySystem::GetHUDWidgetController(const UObject* WorldContextObject) {
	FWidgetControllerParams Params;
	ACpp_HUD_RPG* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, Params, HUD)) {
		return HUD->GetHUDWidgetController(Params);
	}
	return nullptr;
}
UCpp_WidgetController_Attributes* UCpp_BFL_AbilitySystem::GetAttributeWidgetController(const UObject* WorldContextObject) {
	FWidgetControllerParams Params;
	ACpp_HUD_RPG* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, Params, HUD)) {
		return HUD->GetAttributeWidgetController(Params);
	}
	return nullptr;
}
UCpp_WidgetController_Abilities* UCpp_BFL_AbilitySystem::GetAbilitiesWidgetController(const UObject* WorldContextObject) {
	FWidgetControllerParams Params;
	ACpp_HUD_RPG* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, Params, HUD)) {
		return HUD->GetAbilitiesWidgetController(Params);
	}
	return nullptr;
}

void UCpp_BFL_AbilitySystem::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC) {
	// Get Character Class DA & Default Info Struct
	UCpp_DA_CharacterClassInfo* CharacterClassInfo = GetCharacterClassInfoDA(WorldContextObject);
	FCharacterClassDefaultInfo DefaultInfo = CharacterClassInfo->GetCharacterClassDefaults(CharacterClass);

	// Create Effect Context & Setup Source Object
	FGameplayEffectContextHandle AttributesContextHandle = ASC->MakeEffectContext();
	AActor* AvatarActor = ASC->GetAvatarActor();
	AttributesContextHandle.AddSourceObject(AvatarActor);

	// Apply Primary Attributes
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		DefaultInfo.PrimaryAttributes, Level, AttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// Apply Secondary Attributes
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes, Level, AttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// Apply Vital Attributes
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, Level, AttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UCpp_BFL_AbilitySystem::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClass CharacterClass) {
	// Get Character Class DA
	UCpp_DA_CharacterClassInfo* ClassInfo = GetCharacterClassInfoDA(WorldContextObject);
	if (ClassInfo == nullptr) {
		return;
	}

	for (const auto& Ability : ClassInfo->CommonAbilities) {
		ASC->GiveAbility(FGameplayAbilitySpec(Ability, 1));
	}

	const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->GetCharacterClassDefaults(CharacterClass);

	for (const auto& Ability : DefaultInfo.StartupAbilities) {
		// Get enemy level
		const auto& AvatarActor = ASC->GetAvatarActor();
		if (AvatarActor->Implements<UCpp_CombatInterface>()) {
			ASC->GiveAbility(FGameplayAbilitySpec(Ability, ICpp_CombatInterface::Execute_GetPlayerLevel(AvatarActor)));
		}
	}
}

UCpp_DA_CharacterClassInfo* UCpp_BFL_AbilitySystem::GetCharacterClassInfoDA(const UObject* WorldContextObject) {
	const ACpp_GM_RPG* GM = Cast<ACpp_GM_RPG>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GM) {
		return nullptr;
	}
	return GM->CharacterClassInfo;
}
UCpp_DA_AbilityInfo* UCpp_BFL_AbilitySystem::GetAbilityInfoDA(const UObject* WorldContextObject) {
	const ACpp_GM_RPG* GM = Cast<ACpp_GM_RPG>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GM) {
		return nullptr;
	}
	return GM->AbilityInfo;
}

bool UCpp_BFL_AbilitySystem::GetIsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->IsBlockedHit();
	}
	return false;
}
void UCpp_BFL_AbilitySystem::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
											 const bool bIsBlockedHit) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetBlockedHit(bIsBlockedHit);
	}
}

bool UCpp_BFL_AbilitySystem::GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->IsCriticalHit();
	}
	return false;
}
void UCpp_BFL_AbilitySystem::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
											  const bool bIsCriticalHit) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetCriticalHit(bIsCriticalHit);
	}
}

bool UCpp_BFL_AbilitySystem::GetIsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->IsSuccessfulDebuff();
	}
	return false;
}
float UCpp_BFL_AbilitySystem::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->GetDebuffDamage();
	}
	return 0.f;
}
float UCpp_BFL_AbilitySystem::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->GetDebuffFrequency();
	}
	return 0.f;
}
float UCpp_BFL_AbilitySystem::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		return RPGContext->GetDebuffDuration();
	}
	return 0.f;
}
FGameplayTag UCpp_BFL_AbilitySystem::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<const FGameplayEffectContext_RPG*>(
		EffectContextHandle.Get())) {
		if (RPGContext->GetDamageType().IsValid()) {
			return *RPGContext->GetDamageType();
		}
	}
	return FGameplayTag::EmptyTag;
}

void UCpp_BFL_AbilitySystem::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsSuccessfulDebuff) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetIsSuccessfulDebuff(bIsSuccessfulDebuff);
	}
}
void UCpp_BFL_AbilitySystem::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, const float DebuffDamage) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetDebuffDamage(DebuffDamage);
	}
}
void UCpp_BFL_AbilitySystem::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, const float DebuffFrequency) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetDebuffFrequency(DebuffFrequency);
	}
}
void UCpp_BFL_AbilitySystem::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, const float DebuffDuration) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetDebuffDuration(DebuffDuration);
	}
}
void UCpp_BFL_AbilitySystem::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		RPGContext->SetDamageType(DamageType);
	}
}

FVector UCpp_BFL_AbilitySystem::GetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		return RPGContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}
void UCpp_BFL_AbilitySystem::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& NewDeathImpulse) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetDeathImpulse(NewDeathImpulse);
	}
}

FVector UCpp_BFL_AbilitySystem::GetKnockbackImpulse(FGameplayEffectContextHandle& EffectContextHandle) {
	if (const FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		return RPGContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}
void UCpp_BFL_AbilitySystem::SetKnockbackImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& NewKnockbackImpulse) {
	if (FGameplayEffectContext_RPG* RPGContext = static_cast<FGameplayEffectContext_RPG*>(EffectContextHandle.Get())) {
		RPGContext->SetKnockbackImpulse(NewKnockbackImpulse);
	}
}

void UCpp_BFL_AbilitySystem::GetAlivePlayersWithinRadius(const UObject* WorldContextObject,
														 TArray<AActor*>& OutActors,
														 UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
														 const FVector& Origin,
														 const float Radius) {
	// Create collision query params
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bTraceComplex = false;

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
																 EGetWorldErrorMode::LogAndReturnNull)) {
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity,
										FCollisionObjectQueryParams::AllDynamicObjects,
										FCollisionShape::MakeSphere(Radius), Params);
		for (const auto& Overlap : Overlaps) {
			// if implements the combat interface and is not dead
			if (ICpp_CombatInterface* CombatActor = Cast<ICpp_CombatInterface>(Overlap.GetActor())) {
				if (!CombatActor->Execute_IsDead(Overlap.GetActor())) {
					OutActors.AddUnique(Overlap.GetActor());
				}
			}
		}
	}
}
void UCpp_BFL_AbilitySystem::GetClosestTarget(int32 MaxNumberOfTargets,
											  const TArray<AActor*>& Actors,
											  TArray<AActor*>& OutClosestTargets,
											  FVector Origin) {
	// If the number of actors is less than the max number of targets, return all actors
	if (Actors.Num() <= MaxNumberOfTargets) {
		OutClosestTargets = Actors;
		return;
	}
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumOfTargetsFound = 0;

	while(NumOfTargetsFound < MaxNumberOfTargets) {
		double ClosestDistance = TNumericLimits<double>::Max();		
		AActor* ClosestActor = nullptr;
		for (AActor* Actor : ActorsToCheck) {
			const double Distance = FVector::DistSquared(Origin, Actor->GetActorLocation());
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;				
				ClosestActor = Actor;
			}
		}
		// Add the closest actor to the output array and remove it from the actors to check
		OutClosestTargets.AddUnique(ClosestActor);
		ActorsToCheck.Remove(ClosestActor);
		NumOfTargetsFound++;
	}	


}
bool UCpp_BFL_AbilitySystem::IsNotFriend(const AActor* ActorA, const AActor* ActorB) {
	if (ActorA->ActorHasTag("Player") && ActorB->ActorHasTag("Player")) {
		return false;
	}
	if (ActorA->ActorHasTag("Enemy") && ActorB->ActorHasTag("Enemy")) {
		return false;
	}
	return true;
}

TArray<FRotator> UCpp_BFL_AbilitySystem::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumberOfRotaters) {
	
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumberOfRotaters > 1) {
		const float DeltaSpread = Spread / (NumberOfRotaters - 1);
		for (uint8 i = 0; i < NumberOfRotaters; i++) {
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else {
		Rotators.Add(ForwardVector.Rotation());
	}
	return Rotators;
}
TArray<FVector> UCpp_BFL_AbilitySystem::EvenlyRotatedVectors(const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumberOfVectors) {

	TArray<FVector> Vectors;

	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumberOfVectors > 1) {
		const float DeltaSpread = Spread / (NumberOfVectors - 1);
		for (uint8 i = 0; i < NumberOfVectors; i++) {
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else {
		Vectors.Add(ForwardVector);
	}
	return Vectors;
}

int32 UCpp_BFL_AbilitySystem::GetExpForKillOnClassAndLevel(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level) {
	// Get Character Class DA
	UCpp_DA_CharacterClassInfo* ClassInfo = GetCharacterClassInfoDA(WorldContextObject);
	if (ClassInfo == nullptr) {
		return 0;
	}

	// Get Class Info
	FCharacterClassDefaultInfo Info = ClassInfo->GetCharacterClassDefaults(CharacterClass);
	// Get Experience based on Level and Class
	int Exp = static_cast<int>(Info.ExperienceOnKill.GetValueAtLevel(Level));
	return Exp;
}

FGameplayEffectContextHandle UCpp_BFL_AbilitySystem::ApplyDamageEffectWithDamageParams(const FDamageEffectParams& Params) {
	// Create Effect Context
	FGameplayEffectContextHandle DamageContextHandle = Params.SourceASC->MakeEffectContext();
	DamageContextHandle.AddSourceObject(Params.SourceASC->GetAvatarActor());
	
	SetDeathImpulse(DamageContextHandle, Params.DeathImpulse);
	SetKnockbackImpulse(DamageContextHandle, Params.KnockbackImpulse);
	
	// Create Effect Spec
	const FGameplayEffectSpecHandle DamageSpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, DamageContextHandle);

	// Assign Set By Caller Magnitude For Base Damage
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);
	// Assign Set By Caller Magnitude For Debuffs
	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, AllTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, AllTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, AllTags.Debuff_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, AllTags.Debuff_Duration, Params.DebuffDuration);

	Params.TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
	return DamageContextHandle;
}

bool UCpp_BFL_AbilitySystem::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, ACpp_HUD_RPG*& OutHud) {
	// Get the player controller
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
		// Get Hud
		OutHud = Cast<ACpp_HUD_RPG>(PC->GetHUD());
		if (OutHud) {
			// Get Player State, Ability System Component, Attribute Set
			ACpp_PS_Player* PS = PC->GetPlayerState<ACpp_PS_Player>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* ASet = PS->GetAttributeSet();

			// Set the OutParams
			OutParams.PlayerController = PC;
			OutParams.PlayerState = PS;
			OutParams.AbilitySystemComponent = ASC;
			OutParams.AttributeSet = ASet;
			return true;
		}
	}
	return false;
}