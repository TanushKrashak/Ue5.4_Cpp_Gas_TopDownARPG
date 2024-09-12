// No Copyright!

// Game Includes
#include "AbilitySystem/Abilities/Passive/Cpp_GA_RPG_Passive.h"
#include "AbilitySystem/Cpp_ASC_Master.h"

// Engine Includes 
#include "AbilitySystemBlueprintLibrary.h"

void UCpp_GA_RPG_Passive::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCpp_ASC_Master* ASC = Cast<UCpp_ASC_Master>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))) {
		ASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UCpp_GA_RPG_Passive::ReceiveDeactivate);
	}
}

void UCpp_GA_RPG_Passive::ReceiveDeactivate(const FGameplayTag& AbilityTag) {
	if (AbilityTags.HasTagExact(AbilityTag)) {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
