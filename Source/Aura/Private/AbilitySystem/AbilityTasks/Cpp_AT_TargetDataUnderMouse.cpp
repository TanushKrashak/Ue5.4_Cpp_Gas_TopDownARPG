// No Copyright!

// Game Includes
#include "AbilitySystem/AbilityTasks/Cpp_AT_TargetDataUnderMouse.h"
#include "../Aura.h"

// Engine Includes
#include "AbilitySystemComponent.h"

UCpp_AT_TargetDataUnderMouse* UCpp_AT_TargetDataUnderMouse::CreateTargetDataUnderMouseTask(UGameplayAbility* OwningAbility) {
	UCpp_AT_TargetDataUnderMouse* MyObj = NewAbilityTask<UCpp_AT_TargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UCpp_AT_TargetDataUnderMouse::Activate() {

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled) {
		SendMouseCursorData();
	}
	else {
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictKey = GetActivationPredictionKey();

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictKey).
			AddUObject(this, &UCpp_AT_TargetDataUnderMouse::OnTargetDataReplicatedCallback);
		// This block basically sends the data to the server, if it was sent before the actual event, 
		// it will be stored and sent to the server when the event is triggered later.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,
																										  PredictKey);
		if (!bCalledDelegate) {
			SetWaitingOnRemotePlayerData();
		}
	}
	
}

void UCpp_AT_TargetDataUnderMouse::SendMouseCursorData() {

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// Get Player Controller
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	// Get Location Under Mouse Cursor
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Target, false, HitResult);


	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
														  GetActivationPredictionKey(),
														  DataHandle,
														  FGameplayTag(),
														  AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates()) {
		ValidData.Broadcast(DataHandle);
	}
}

void UCpp_AT_TargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag) {
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates()) {
		ValidData.Broadcast(Data);
	}
}
