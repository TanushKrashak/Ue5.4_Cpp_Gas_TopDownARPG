// No Copyright!

// Game Includes
#include "UI/Cpp_WidgetController.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Core/Cpp_PC_Player.h"
#include "Core/Cpp_PS_Player.h"
#include "AbilitySystem/DataAssets/Cpp_DA_AbilityInfo.h"


void UCpp_WidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params) {
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UCpp_WidgetController::BroadcastInitialValues() {

}

void UCpp_WidgetController::BindCallbacksToDependancies() {

}

void UCpp_WidgetController::BroadcastAbilityInfo() {
	if (!GetCastedASC()->bStartupAbilitiesGiven) {
		return;
	}

	FForEachAbility BroadcastAbilitiesDelegate;
	BroadcastAbilitiesDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec) {
			// get the Ability Info
			FAbilityInfo_RPG AbilInfo = AbilityInfo->FindAbilityInfoByTag(GetCastedASC()->GetAbilityTagFromSpec(AbilitySpec));
			// Set the Input & Status for the Ability Info
			AbilInfo.InputTag = GetCastedASC()->GetInputTagFromSpec(AbilitySpec);
			AbilInfo.StatusTag = GetCastedASC()->GetAbilityStatusFromSpec(AbilitySpec);

			// Broadcast Ability Info
			AbilityInfoDelegate.Broadcast(AbilInfo);
		}
	);
	// Iterate through all abilities and execute the delegate
	GetCastedASC()->ForEachAbility(BroadcastAbilitiesDelegate);
}

ACpp_PC_Player* UCpp_WidgetController::GetCastedPC() {
	if (Casted_PC == nullptr) {
		Casted_PC = Cast<ACpp_PC_Player>(PlayerController);
	}
	return Casted_PC;	
}
ACpp_PS_Player* UCpp_WidgetController::GetCastedPS() {
	if (Casted_PS == nullptr) {
		Casted_PS = Cast<ACpp_PS_Player>(PlayerState);
	}
	return Casted_PS;
}
UCpp_ASC_Master* UCpp_WidgetController::GetCastedASC() {
	if (Casted_ASC == nullptr) {
		Casted_ASC = Cast<UCpp_ASC_Master>(AbilitySystemComponent);
	}
	return Casted_ASC;
}
UCpp_ASet_Master* UCpp_WidgetController::GetCastedASet() {
	if (Casted_ASet == nullptr) {
		Casted_ASet = Cast<UCpp_ASet_Master>(AttributeSet);
	}
	return Casted_ASet;
}
