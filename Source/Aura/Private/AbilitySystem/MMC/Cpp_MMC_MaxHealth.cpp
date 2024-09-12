// No Copyright!


#include "AbilitySystem/MMC/Cpp_MMC_MaxHealth.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "Interfaces/Cpp_CombatInterface.h"

UCpp_MMC_MaxHealth::UCpp_MMC_MaxHealth() {
	VigorDef.AttributeToCapture = UCpp_ASet_Master::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	// This function is used to add the attributes we want to capture to the capture definitions
	RelevantAttributesToCapture.Add(VigorDef);
}

float UCpp_MMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const {
	// Gather Tags From Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Put those tags into aggregation evaluation parameters struct
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(0.0f, Vigor);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCpp_CombatInterface>()) {
		PlayerLevel = ICpp_CombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	Vigor = 80.f + (Vigor * 2.5) + (PlayerLevel * 10);
	return Vigor;
}
