// No Copyright!

// Game Includes
#include "AbilitySystem/ExecCalc/Cpp_ExecCalc_Damage.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "Cpp_AllGameplayTags.h"
#include "AbilitySystem/Cpp_BFL_AbilitySystem.h"
#include "Interfaces/Cpp_CombatInterface.h"
#include "AbilitySystem/DataAssets/Cpp_DA_CharacterClassInfo.h"

// Engine Includes
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "AbilitySystemComponent.h"

struct DamageStatics {
	// Secondary Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	// Resistance Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	DamageStatics() {
		// Function Takes in 4 Parameters, 1st in ASet, 2nd is Attribute, 3rd is Source/Target, 4th is bSnapshot

			// Target Stats
		//===========================================================
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, CriticalHitResistance, Target, false);
		// Resistance To Elements
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, PhysicalResistance, Target, false);
		//===========================================================

			// Source Stats
		//===========================================================
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCpp_ASet_Master, CriticalHitDamage, Source, false);
		//===========================================================
	}
};

static const DamageStatics& GetDamageStatics() {
	static DamageStatics DStatics;
	return DStatics;
}

UCpp_ExecCalc_Damage::UCpp_ExecCalc_Damage() {
		// The Def here is created from the macro above
	//===========================================================
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	// Resistance Attributes
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	//===========================================================
}

void UCpp_ExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	// All Tags -> Capture Def Map for Damage Calculation
	const FAllGameplayTags& FTags = FAllGameplayTags::Get();
	//===========================================================
	// Secondary Attributes
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_Armor, GetDamageStatics().ArmorDef);
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_ArmorPenetration, GetDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_BlockChance, GetDamageStatics().BlockChanceDef);
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_CriticalHitChance, GetDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_CriticalHitDamage, GetDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDef.Add(FTags.Attribute_Secondary_CriticalHitResistance, GetDamageStatics().CriticalHitResistanceDef);
	// Resistance Attributes
	TagsToCaptureDef.Add(FTags.Attribute_Resistance_Fire, GetDamageStatics().FireResistanceDef);
	TagsToCaptureDef.Add(FTags.Attribute_Resistance_Lightning, GetDamageStatics().LightningResistanceDef);
	TagsToCaptureDef.Add(FTags.Attribute_Resistance_Arcane, GetDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDef.Add(FTags.Attribute_Resistance_Physical, GetDamageStatics().PhysicalResistanceDef);
	//===========================================================

		// Required For Any Calculation
	//===========================================================
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Get Level of Source & Target
	int32 SourceLevel = 1;
	if (SourceActor->Implements<UCpp_CombatInterface>()) {
		SourceLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(SourceActor);
	}
	int32 TargetLevel = 1;
	if (TargetActor->Implements<UCpp_CombatInterface>()) {
		TargetLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(TargetActor);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Setup Evaluation Parameters
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	//===========================================================

		// Get Stats
	//===========================================================
	// Get Target Block Chance
	float TargetBlockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Clamp(TargetBlockChance, 0.0f, 100.0f);

	// Get Target Armor
	float TargetArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.0f);

	// Get Target Critical Hit Resistance
	float TargetCriticalHitResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.0f);

	// Get Source Armor Penetration
	float ArmorPenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, ArmorPenetration);
	ArmorPenetration = FMath::Max<float>(ArmorPenetration, 0.0f);

	// Get Source Critical Chance
	float CriticalHitChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, CriticalHitChance);
	CriticalHitChance = FMath::Clamp(CriticalHitChance, 0.0f, 100.0f);

	// Get Source Critical Damage
	float CriticalHitDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
	CriticalHitDamage = FMath::Max<float>(CriticalHitDamage, 0.0f);

	// Get Curve Table for Damage Calculation
	const UCpp_DA_CharacterClassInfo* CharacterClassInfo = UCpp_BFL_AbilitySystem::GetCharacterClassInfoDA(SourceActor);
	check(CharacterClassInfo);

	// Get Armor Penetration
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("Armor Penetration"), FString());
	const float ArmorPenetrationCoef = ArmorPenetrationCurve->Eval(SourceLevel);

	// Get Effective Armor
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("Effective Armor"), FString());
	const float EffectiveArmorCoef = EffectiveArmorCurve->Eval(TargetLevel);

	// Get Critical Hit Resistance
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("Critical Hit Resistance"), FString());
	const float CriticalHitResistanceCoef = CriticalHitResistanceCurve->Eval(TargetLevel);
	//===========================================================

	// Debuff
	DetermineDebuff(Spec, ExecutionParams, EvaluationParameters, TagsToCaptureDef);

	// Total Damage to be Dealt
	float Damage = 0.0f;

	// Go Through Each Damage Type & their Resistance
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAllGameplayTags::Get().DamageTypesToResistances) {
		// Get Dmg, Resistance Tag & Capture Def
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDef[ResistanceTag];

		// Get Damage Value
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		// Get Resistance Value
		float ResistanceValue = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, ResistanceValue);
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.0f, 100.0f);

		// Calculate Damage Reduction
		DamageTypeValue *= (100.0f - ResistanceValue) / 100.0f;
		Damage += DamageTypeValue;
	}

	// Get Effect Context
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

		// Damage Calculation
	//===========================================================
	// If Block, reduce damage by 50%
	const bool bBlock = FMath::FRandRange(0.0f, 100.0f) <= TargetBlockChance;
	UCpp_BFL_AbilitySystem::SetIsBlockedHit(EffectContextHandle, bBlock);
	Damage = bBlock ? Damage * 0.5f : Damage;

	// Take Target Armor and Source Armor Penetration and calculate damage reduction in %
	const float EffectiveArmor = TargetArmor * (100.0f - ArmorPenetration * ArmorPenetrationCoef) / 100.0f;
	Damage *= (100.0f - EffectiveArmor * EffectiveArmorCoef) / 100.0f;

	// If Critical Hit, increase damage by *2 + Critical Hit Damage, CritHitRes reduces CritHitChance
	const float newCriticalHitChance = CriticalHitChance - (TargetCriticalHitResistance * CriticalHitResistanceCoef);
	const bool bCriticalHit = FMath::FRandRange(0.0f, 100.0f) <= newCriticalHitChance;
	UCpp_BFL_AbilitySystem::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	Damage = bCriticalHit ? (Damage * 2) + CriticalHitDamage : Damage;
	//===========================================================

	const FGameplayModifierEvaluatedData EvaluatedData(UCpp_ASet_Master::GetIncomingDamageAttribute(),
													   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UCpp_ExecCalc_Damage::DetermineDebuff(const FGameplayEffectSpec& Spec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDef) const {
	
	const FAllGameplayTags& AllTags = FAllGameplayTags::Get();

	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : AllTags.DamageTypesToDebuffs) {
		// Making it clear what the Tags are
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;

		float TypeDamage = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);

		if (TypeDamage > -0.5) { // -0.5 cuz of floating point errors, just to be safe
			
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(AllTags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.0f;
			const FGameplayTag& ResistanceTag = AllTags.DamageTypesToResistances[DamageTypeTag];
			// Updates TargetDebuffResistance with Target's Resistance for that specific DamageType
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				InTagsToDef[ResistanceTag], EvaluationParameters, TargetDebuffResistance);

			// Remove Negative Value
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);

			// Calculate Effective Debuff Chance and if Debuff is successful
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.0f;
			const bool bDebuff = FMath::FRandRange(0.0f, 100.0f) < EffectiveDebuffChance;
			if (bDebuff) {
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UCpp_BFL_AbilitySystem::SetIsSuccessfulDebuff(ContextHandle, true);
				UCpp_BFL_AbilitySystem::SetDamageType(ContextHandle, DamageTypeTag);

				// Get Debuff Frequency, Duration and Damage
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(AllTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(AllTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(AllTags.Debuff_Frequency, false, -1.f);

				// Set Debuff Frequency, Duration and Damage
				UCpp_BFL_AbilitySystem::SetDebuffDamage(ContextHandle, DebuffDamage);
				UCpp_BFL_AbilitySystem::SetDebuffDuration(ContextHandle, DebuffDuration);
				UCpp_BFL_AbilitySystem::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}