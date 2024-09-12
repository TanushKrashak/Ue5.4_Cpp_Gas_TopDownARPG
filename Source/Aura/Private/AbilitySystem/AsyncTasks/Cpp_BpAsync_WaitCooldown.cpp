// No Copyright!

// Game Includes
#include "AbilitySystem/AsyncTasks/Cpp_BpAsync_WaitCooldown.h"

// Engine Includes
#include "AbilitySystemComponent.h"


UCpp_BpAsync_WaitCooldown* UCpp_BpAsync_WaitCooldown::WaitForCooldown(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTags) {
	// create a new instance of the class
	UCpp_BpAsync_WaitCooldown* WaitCooldown = NewObject<UCpp_BpAsync_WaitCooldown>();

	WaitCooldown->ASC = AbilitySystemComponent;
	WaitCooldown->CooldownTag = InCooldownTags;
	if (!IsValid(AbilitySystemComponent) || !InCooldownTags.IsValid()) {
		WaitCooldown->EndTask();
		return nullptr;
	}
	// to know when the cooldown effect is removed
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTags, EGameplayTagEventType::NewOrRemoved).
		AddUObject(WaitCooldown, &UCpp_BpAsync_WaitCooldown::CooldownTagChanged);

	// To know when the cooldown effect is added
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldown, &UCpp_BpAsync_WaitCooldown::OnActiveEffectAdded);

	return WaitCooldown;
}

void UCpp_BpAsync_WaitCooldown::EndTask() {
	if (!IsValid(ASC)) {
		return;
	}
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// Mark the object as ready to destroy and then mark it as garbage
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCpp_BpAsync_WaitCooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount) {
	if (NewCount == 0) {
		CooldownEnd.Broadcast(0.0f);
	}
}

void UCpp_BpAsync_WaitCooldown::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle) {
	
	// Get Asset Tags and Granted Tags
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// Check if the cooldown tag is in the granted tags
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag)) {
		// Get the remaining time of the cooldown effect
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		// as its a single tag, TimesRemaining should have only one element, but we check it anyway
		if (TimesRemaining.Num() > 0) {
			// Find the highest time remaining			
			float HighestTime = TimesRemaining[0];
			for (const auto& Time : TimesRemaining) {
				HighestTime = FMath::Max(HighestTime, Time);
			}
			CooldownStart.Broadcast(HighestTime);
		}
	}
}
