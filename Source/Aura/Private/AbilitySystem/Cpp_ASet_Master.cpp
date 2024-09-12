// No Copyright!

// Game Includes
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "Cpp_AllGameplayTags.h"
#include "Interfaces/Cpp_CombatInterface.h"
#include "Core/Cpp_PC_Player.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "Interfaces/Cpp_PlayerInterface.h"
#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"

// Engine Includes
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



UCpp_ASet_Master::UCpp_ASet_Master() {
	const FAllGameplayTags& GameplayTags = FAllGameplayTags::Get();

	// Primary Attributes
	TagToAttributes.Add(GameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Primary_Resilience, GetResilienceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Primary_Vigor, GetVigorAttribute);

	// Secondary Attributes
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_Armor, GetArmorAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);

	// Resistance Attributes
	TagToAttributes.Add(GameplayTags.Attribute_Resistance_Fire, GetFireResistanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Resistance_Lightning, GetLightningResistanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagToAttributes.Add(GameplayTags.Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UCpp_ASet_Master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Registers the attributes to be replicated
    //===========================================================
    // Vital Attributes
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Mana, COND_None, REPNOTIFY_Always);

    // Primary Attributes
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Resilience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Vigor, COND_None, REPNOTIFY_Always);
    
    // Secondary Attributes
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, ArmorPenetration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, BlockChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, CriticalHitChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, CriticalHitDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, CriticalHitResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, HealthRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, ManaRegeneration, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCpp_ASet_Master, PhysicalResistance, COND_None, REPNOTIFY_Always);
    //===========================================================
}

void UCpp_ASet_Master::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute()) {
		NewValue = FMath::Max<float>(0.0f, NewValue);
	}
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute()) {
		NewValue = FMath::Max<float>(0.0f, NewValue);
	}
}
void UCpp_ASet_Master::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	
	// Source = Causer of the effect, Target = Target of the effect (owner of the AttributeSet)
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Check if the target is dead
	if (Props.TargetCharacter->Implements<UCpp_CombatInterface>() && ICpp_CombatInterface::Execute_IsDead(Props.TargetCharacter)) {
		return;		
	}

	// Clamping Health and Mana
	{
		if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		}
		else if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
			SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
		}
	}	

	// Damage Calculation	
	{
		if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
			const float LocalIncomingDamage = GetIncomingDamage();
			SetIncomingDamage(0.0f);
			if (LocalIncomingDamage > 0.0f) {
				// Calculate Damage Reduction
				const float newHealth = GetHealth() - LocalIncomingDamage;
				SetHealth(FMath::Clamp(newHealth, 0.0f, GetMaxHealth()));

				const bool bFatal = newHealth <= 0.0f;
				// Die				
				if (bFatal) {
					ICpp_CombatInterface* CombatInterface = Cast<ICpp_CombatInterface>(Props.TargetAvatarActor);
					if (CombatInterface) {
						CombatInterface->Die(UCpp_BFL_AbilitySystem::GetDeathImpulse(Props.EffectContextHandle));
					}
					SendExperienceOnDeath(Props);
				}

				else {
					// Do not apply hit react if the target is being Shocked
					if (Props.TargetCharacter->Implements<UCpp_CombatInterface>() && !ICpp_CombatInterface::
						Execute_GetIsBeingShocked(Props.TargetCharacter)) {
						// Apply Hit React Tag
						FGameplayTagContainer TagContainer;
						TagContainer.AddTag(FAllGameplayTags::Get().Effect_HitReact);
						Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
					}

					const FVector KnockbackImpulse = UCpp_BFL_AbilitySystem::GetKnockbackImpulse(Props.EffectContextHandle);
					
					if (!KnockbackImpulse.IsNearlyZero(1.f)) {
						Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
						Props.TargetCharacter->LaunchCharacter(KnockbackImpulse, true, true);
					}
				}

				const bool bBlockedHit = UCpp_BFL_AbilitySystem::GetIsBlockedHit(Props.EffectContextHandle);
				const bool bIsCriticalHit = UCpp_BFL_AbilitySystem::GetIsCriticalHit(Props.EffectContextHandle);
				// Show Damage Number Widget
				ShowDamageText(Props, LocalIncomingDamage, bBlockedHit, bIsCriticalHit);


				// Check if the effect is a debuff
				if (UCpp_BFL_AbilitySystem::GetIsSuccessfulDebuff(Props.EffectContextHandle)) {
					FGameplayEffectContextHandle ContextHandle = Props.SourceASC->MakeEffectContext();
					ContextHandle.AddSourceObject(Props.SourceAvatarActor);

					const FAllGameplayTags& AllTags = FAllGameplayTags::Get();

					// Get Debuff Details
					const FGameplayTag DamageType = UCpp_BFL_AbilitySystem::GetDamageType(Props.EffectContextHandle);
					const float DebuffDamage = UCpp_BFL_AbilitySystem::GetDebuffDamage(Props.EffectContextHandle);
					const float DebuffDuration = UCpp_BFL_AbilitySystem::GetDebuffDuration(Props.EffectContextHandle);
					const float DebuffFrequency = UCpp_BFL_AbilitySystem::GetDebuffFrequency(Props.EffectContextHandle);
					const FGameplayTag DebuffTag = AllTags.DamageTypesToDebuffs[DamageType];

					FString DebuffName = FString::Printf(TEXT("Dynamic Debuff_%s"), *DebuffTag.ToString());
					// Create Gameplay Effect for debuff dynamically
					UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

					//Effect->InheritableOwnedTagsContainer.AddTag(AllTags.DamageTypesToDebuffs[DamageType]); IS Deprecated
					FInheritedTagContainer TagContainer = FInheritedTagContainer();
					UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

					// Block all other inputs when shocked
					if (DebuffTag.MatchesTagExact(AllTags.Debuff_Shock)) {
						TagContainer.Added.AddTag(AllTags.Player_Block_CursorTrace);
						TagContainer.Added.AddTag(AllTags.Player_Block_Input_Held);
						TagContainer.Added.AddTag(AllTags.Player_Block_Input_Pressed);
						TagContainer.Added.AddTag(AllTags.Player_Block_Input_Released);

						// If the debuff was valid, stop the hit react animation
						/*if (Props.TargetCharacter) {
							Props.TargetCharacter->StopAnimMontage();
						}*/
					}

					// Set Effect Timer Properties
					Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
					Effect->Period = DebuffFrequency;
					Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
				
					// Add Debuff Tag
					TagContainer.Added.AddTag(DebuffTag);
					//TagContainer.CombinedTags.AddTag(AllTags.DamageTypesToDebuffs[DamageType]);
					
					// Apply the tag changes
					Component.SetAndApplyTargetTagChanges(TagContainer);

					// Set Effect Modifiers
					Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
					Effect->StackLimitCount = 1;

                    // Add debuff damage modifier
                    const int32 Index = Effect->Modifiers.Num();
                    Effect->Modifiers.Add(FGameplayModifierInfo());
                    FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

					// Set Modifier Properties
                    ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
                    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
                    ModifierInfo.Attribute = UCpp_ASet_Master::GetIncomingDamageAttribute();

                    // Create a new gameplay effect spec for the debuff effect
                    if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, ContextHandle, 1)) {
                        FGameplayEffectContext_RPG* RpgContext = static_cast<FGameplayEffectContext_RPG*>(MutableSpec->GetContext().Get());
                        TSharedPtr<FGameplayTag> DebuffDamageTag = MakeShareable(new FGameplayTag(DamageType));
                        RpgContext->SetDamageType(DebuffDamageTag);
																	
						// Apply Debuff
						Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
                    }
				}
		
			}
		}
	}	

	// Experience Calculation
	{
		if (Data.EvaluatedData.Attribute == GetIncomingExperienceAttribute()) {
			const float LocalIncomingExperience = GetIncomingExperience();
			SetIncomingExperience(0.0f);

			if (Props.SourceCharacter->Implements<UCpp_PlayerInterface>() &&
				Props.SourceCharacter->Implements<UCpp_CombatInterface>()) {

				// Get Current Level and Experience
				const int32 CurrentLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
				const int32 CurrentExperience = ICpp_PlayerInterface::Execute_GetExperience(Props.SourceCharacter);

				const int32 NewLevel = ICpp_PlayerInterface::Execute_FindLevelForExperience(Props.SourceCharacter, CurrentExperience + LocalIncomingExperience);

				const int32 NumberOfLevelsGained = NewLevel - CurrentLevel;
				if (NumberOfLevelsGained > 0) {
					// Update Player Level
					ICpp_PlayerInterface::Execute_UpdatePlayerLevel(Props.SourceCharacter, NumberOfLevelsGained);
					
					// Get Attribute and Spell Points Rewarded
					int32 AttributePointsReward = 0;
					int32 SpellPointsReward = 0;
					for (uint8 i = 0; i < NumberOfLevelsGained; i++) {
						AttributePointsReward += ICpp_PlayerInterface::Execute_GetAttributPointsRewardForLevelUp(Props.SourceCharacter, CurrentLevel + i);
						SpellPointsReward += ICpp_PlayerInterface::Execute_GetSpellPointsRewardForLevelUp(Props.SourceCharacter, CurrentLevel + i);
					}

					// Update Attribute Points and Spell Points
					ICpp_PlayerInterface::Execute_UpdatePlayerAttributePoints(Props.SourceCharacter, AttributePointsReward);
					ICpp_PlayerInterface::Execute_UpdatePlayerSpellPoints(Props.SourceCharacter, SpellPointsReward);

					// Fill up Health and Mana
					bTopOffHealth = true;
					bTopOffMana = true;

					ICpp_PlayerInterface::Execute_LevelUp(Props.SourceCharacter);
				}

				ICpp_PlayerInterface::Execute_UpdateExperience(Props.SourceCharacter, LocalIncomingExperience);
			}
		}
	}
	
}

void UCpp_ASet_Master::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth) {
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	else if (Attribute == GetMaxManaAttribute() && bTopOffMana) {
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

// Vital Attributes RepNotify
void UCpp_ASet_Master::OnRep_Health(const FGameplayAttributeData& OldHealth) {
	// Let's GAS know that Health is being replicated
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Health, OldHealth);


}
void UCpp_ASet_Master::OnRep_Mana(const FGameplayAttributeData& OldMana) {
	// Let's GAS know that Health is being replicated
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Mana, OldMana);
	
}

// Primary Attributes RepNotify
void UCpp_ASet_Master::OnRep_Strength(const FGameplayAttributeData& OldStrength) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Strength, OldStrength);
}
void UCpp_ASet_Master::OnRep_Resilience(const FGameplayAttributeData& OldResilience) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Resilience, OldResilience);
}
void UCpp_ASet_Master::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Intelligence, OldIntelligence);
}
void UCpp_ASet_Master::OnRep_Vigor(const FGameplayAttributeData& OldVigor) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Vigor, OldVigor);
}

// Secondary Attributes RepNotify
void UCpp_ASet_Master::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
	// Let's GAS know that Health is being replicated
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, MaxHealth, OldMaxHealth);
}
void UCpp_ASet_Master::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) {
	// Let's GAS know that Health is being replicated
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, MaxMana, OldMaxMana);
}
void UCpp_ASet_Master::OnRep_Armor(const FGameplayAttributeData& OldArmor) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, Armor, OldArmor);
}
void UCpp_ASet_Master::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, ArmorPenetration, OldArmorPenetration);
}
void UCpp_ASet_Master::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, BlockChance, OldBlockChance);
}
void UCpp_ASet_Master::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, CriticalHitChance, OldCriticalHitChance);
}
void UCpp_ASet_Master::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, CriticalHitDamage, OldCriticalHitDamage);
}
void UCpp_ASet_Master::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, CriticalHitResistance, OldCriticalHitResistance);
}
void UCpp_ASet_Master::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, HealthRegeneration, OldHealthRegeneration);
}
void UCpp_ASet_Master::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, ManaRegeneration, OldManaRegeneration);
}

// Resistance Attributes RepNotify
void UCpp_ASet_Master::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, FireResistance, OldFireResistance);
}
void UCpp_ASet_Master::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, LightningResistance, OldLightningResistance);
}
void UCpp_ASet_Master::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, ArcaneResistance, OldArcaneResistance);
}
void UCpp_ASet_Master::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCpp_ASet_Master, PhysicalResistance, OldPhysicalResistance);
}
 
void UCpp_ASet_Master::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const {
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		// SourceController is nullptr if the source is not a player
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController) {
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UCpp_ASet_Master::ShowDamageText(const FEffectProperties& Props, float DamageAmount, const bool bBlockedHit, 
									  const bool bIsCriticalHit) const {
	if (Props.SourceCharacter != Props.TargetCharacter) {
		if (auto PC = Cast<ACpp_PC_Player>(Props.SourceCharacter->Controller)) {
			PC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bIsCriticalHit);
		}
		else if (auto TargetPC = Cast<ACpp_PC_Player>(Props.TargetCharacter->Controller)) {
			TargetPC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bIsCriticalHit);
		}
	}
}

void UCpp_ASet_Master::SendExperienceOnDeath(const FEffectProperties& Props) const {
	
	if (Props.TargetAvatarActor->Implements<UCpp_CombatInterface>()) {
		const int32 TargetLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICpp_CombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);

		const int32 ExpReward = UCpp_BFL_AbilitySystem::GetExpForKillOnClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);
		
		// Setup Payload & Get GameplayTags
		const FAllGameplayTags& GameplayTags = FAllGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingExperience;
		Payload.EventMagnitude = ExpReward;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attribute_Meta_IncomingExperience, Payload);
	}
	 
}
