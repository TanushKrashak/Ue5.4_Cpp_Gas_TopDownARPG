// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

//====================================================================================================
// This class is a container for all the gameplay tags in the game.
// Its a Singleton class that holds all the native gameplay tags in the game.
//====================================================================================================

struct FAllGameplayTags {
public:
	//================================================================================================================
	// PROTECTED FUNCTIONS
	//================================================================================================================
	// Primary Attributes
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Vigor;

	// Secondary Attributes
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;

	// Meta Attributes
	FGameplayTag Attribute_Meta_IncomingExperience;


	// Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;


	// Damage Tags
	//==========================================================
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	// Damage Resistance Tags
	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;

	// Debuff Tags
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Shock;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Curse;
	
	// Debuff Property Tags
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Duration;

	// Damage Mappings
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	//==========================================================
	

	// Effect Tags
	FGameplayTag Effect_HitReact;


	// Ability Tags
	//==========================================================
	// Base Case, only used for the default value or checks
	FGameplayTag Ability_None;

	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;
	FGameplayTag Ability_HitReact;
	
	// Spell Abilities
	FGameplayTag Ability_Fireball;
	FGameplayTag Ability_Electrocute;

	// Status
	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;
	// Types
	FGameplayTag Ability_Type_Offensive;
	FGameplayTag Ability_Type_Passive;	
	FGameplayTag Ability_Type_None;	

	// Passive Abilities
	FGameplayTag Ability_Passive_HaloOfProtection;
	FGameplayTag Ability_Passive_LifeSiphon;
	FGameplayTag Ability_Passive_ManaSiphon;
	//==========================================================

	// Ability Cooldown Tags
	FGameplayTag Cooldown_Ability_Fireball;

	// Combat Socket Tags
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightFoot;
	FGameplayTag CombatSocket_LeftFoot;
	FGameplayTag CombatSocket_Tail;

	// Montage Attack Tags
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Attack_5;

	// Player Tags
	FGameplayTag Player_Block_Input_Pressed;
	FGameplayTag Player_Block_Input_Held;
	FGameplayTag Player_Block_Input_Released;
	FGameplayTag Player_Block_CursorTrace;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	static const FAllGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();


protected:
	//================================================================================================================
	//================================================================================================================
	static FAllGameplayTags GameplayTags;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

};