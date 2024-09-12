// No Copyright!


#include "Cpp_AllGameplayTags.h"
#include "GameplayTagsManager.h"

FAllGameplayTags FAllGameplayTags::GameplayTags;

void FAllGameplayTags::InitializeNativeGameplayTags() {
		// Primary Attributes
	//========================================================== 
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases Physical Damage"));

	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration"));

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases Mana Capacity and Damage"));

	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health Capacity"));
	//========================================================== 


		// Secondary Attributes
	//========================================================== 
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum Health"));

	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum Mana"));

	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces Damage Taken, Improves Block Chance"));

	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignored Percentage Of Enemy Armor, Increases Critical Hit Chance"));

	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half"));

	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to deal double damage plus critical damage bonus"));

	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus Damage dealt by critical hits"));

	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces the chance of enemies getting Critical Hits"));

	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second"));

	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second"));
	//========================================================== 


	// Meta Attributes
	//==========================================================
	GameplayTags.Attribute_Meta_IncomingExperience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingExperience"),
		FString("Incoming Experience Meta Attribute"));
	//==========================================================


		// Input Tags
	//========================================================== 
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Right Mouse Button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input 1 Key"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input 2 Key"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input 3 Key"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input 4 Key"));

	// Passive Inputs
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input Passive 1, only for Logic, not a key"));

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Passive 2, only for Logic, not a key"));
	//========================================================== 


		// Damage Types & Their Respective Resistances & Debuffs
	//==========================================================
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage Dealt"));		

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Dealt"));
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Fire Damage Resistance"));
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff For Fire Attacks"));
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Dealt"));
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Lightning Damage Resistance"));
	GameplayTags.Debuff_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Shock"),
		FString("Debuff For Lightning Attacks"));
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Shock);

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Dealt"));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Arcane Damage Resistance"));
	GameplayTags.Debuff_Curse = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Curse"),
		FString("Debuff For Arcane Attacks"));
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Curse);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Dealt"));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Physical Damage Resistance"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff For Physical Attacks"));
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Stun);


	// Debuff Property Tags
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Chance to Apply Debuff"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Amount of Damage Dealt by Debuff"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("How Often Debuff is Applied"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("How Long Debuff Lasts"));
	//==========================================================


		// Effect Tags
	//==========================================================
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("Tag Granted for Hit Reaction"));
	//==========================================================


		// Ability Tags
	//==========================================================
	// Base Ability
	GameplayTags.Ability_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.None"),
		FString("No Ability - like the nullptr but for Tags"));

	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Attack"),
		FString("Attack Ability Tag"));
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Summon"),
		FString("Summon Ability Tag"));
	GameplayTags.Ability_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.HitReact"),
		FString("Hit React Ability Tag"));

	// Spell Abilities
	GameplayTags.Ability_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fireball"),
		FString("Fireball Ability Tag"));
	GameplayTags.Ability_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Electrocute"),
		FString("Electrocute Ability Tag"));

	// Status
	GameplayTags.Ability_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Status.Locked"),
		FString("Ability is Locked"));
	GameplayTags.Ability_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Status.Eligible"),
		FString("Ability is Eligible To Be Unlocked"));
	GameplayTags.Ability_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Status.Unlocked"),
		FString("Ability is Unlocked"));
	GameplayTags.Ability_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Status.Equipped"),
		FString("Ability is Equipped"));

	// Types
	GameplayTags.Ability_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Type.Offensive"),
		FString("Offensive Ability"));
	GameplayTags.Ability_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Type.Passive"),
		FString("Passive Ability"));
	GameplayTags.Ability_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Type.None"),
		FString("No Ability Type"));

	// Pasive Abilities
	GameplayTags.Ability_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Passive.HaloOfProtection"),
		FString("Passive Ability Tag For Ability Halo Of Protection"));
	GameplayTags.Ability_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Passive.LifeSiphon"),
		FString("Passive Ability Tag For Ability Life Siphon"));
	GameplayTags.Ability_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Passive.ManaSiphon"),
		FString("Passive Ability Tag For Ability Mana Siphon"));
	
	//==========================================================


		// Ability Cooldown Tags
	//==========================================================
	GameplayTags.Cooldown_Ability_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ability.Fireball"),
		FString("Cooldown Tag for Fireball Ability"));
	//==========================================================



		// Combat Socket Tags
	//==========================================================
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Tag Granted for Weapon Attack Montages"));

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Tag Granted for Right Hand Attack Montages"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Tag Granted for Left Hand Attack Montages"));

	GameplayTags.CombatSocket_RightFoot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightFoot"),
		FString("Tag Granted for Right Foot Attack Montages"));

	GameplayTags.CombatSocket_LeftFoot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftFoot"),
		FString("Tag Granted for Left Foot Attack Montages"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tag Granted for Tail Attack Montages"));
	//==========================================================


		// Montage Attack Tags
	//==========================================================
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Tag Granted for Attack Montage 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Tag Granted for Attack Montage 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Tag Granted for Attack Montage 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Tag Granted for Attack Montage 4"));
	GameplayTags.Montage_Attack_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.5"),
		FString("Tag Granted for Attack Montage 5"));
	//==========================================================


	// Player Tags
	//==========================================================
	GameplayTags.Player_Block_Input_Pressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.Input.Pressed"),
		FString("Block Input Pressed Callback For Input"));	
	GameplayTags.Player_Block_Input_Held = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.Input.Held"),
		FString("Block Input Released Callback For Input"));
	GameplayTags.Player_Block_Input_Released = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.Input.Released"),
		FString("Block Input Released Callback For Input"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block tracing under the cursor"));
	//==========================================================
}
