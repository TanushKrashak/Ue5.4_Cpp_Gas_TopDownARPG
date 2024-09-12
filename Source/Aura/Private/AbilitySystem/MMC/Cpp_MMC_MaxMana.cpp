// No Copyright!


#include "AbilitySystem/MMC/Cpp_MMC_MaxMana.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "Interfaces/Cpp_CombatInterface.h"

UCpp_MMC_MaxMana::UCpp_MMC_MaxMana() {
	IntelligenceDef.AttributeToCapture = UCpp_ASet_Master::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	// This function is used to add the attributes we want to capture to the capture definitions
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UCpp_MMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const {
	// Gather Tags From Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Put those tags into aggregation evaluation parameters struct
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(0.0f, Intelligence);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCpp_CombatInterface>()) {
		PlayerLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	Intelligence = 50.f + (Intelligence * 2.5f) + (PlayerLevel * 15.f);
	return Intelligence;
}
