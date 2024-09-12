// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Cpp_GameplayAbility_RPG.h"
#include "AbilitySystem/Cpp_ASet_Master.h"

FString UCpp_GameplayAbility_RPG::GetDescriptionForLevel(int32 Level, bool bNextLevel /* = false */) const {
	return FString::Printf(TEXT("<Title>Your Ability is At Level:</> %d"), Level);
}
FString UCpp_GameplayAbility_RPG::GetLockedDescription(int32 Level) {
	return FString::Printf(TEXT("<Title>Your Ability Will be At Level:</> %d"), Level);
}
 
int32 UCpp_GameplayAbility_RPG::GetManaCostAtLevel(int32 Level /*= 1*/) const {
	float cost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect()) {
		for (const auto& Mod : CostEffect->Modifiers) {
			if (Mod.Attribute.IsValid() && Mod.Attribute == UCpp_ASet_Master::GetManaAttribute()) {
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, cost);
				break;
			}
		}
	}
	return static_cast<int>(cost);
}
float UCpp_GameplayAbility_RPG::GetCooldownAtLevel(int32 Level /*= 1*/) const {
	float cooldown = 0.0f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect()) {
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, cooldown);
	}
	return cooldown;
}
