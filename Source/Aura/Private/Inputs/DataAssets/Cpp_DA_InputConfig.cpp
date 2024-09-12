// No Copyright!


#include "Inputs/DataAssets/Cpp_DA_InputConfig.h"

const UInputAction* UCpp_DA_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag) const {
	// Iterate through the AbilityInputActions array and check if the InputTag matches the Tag
	for (const FRPGInputAction& InputAction : AbilityInputActions) {
		if (InputAction.InputTag.MatchesTagExact(Tag)) {
			return InputAction.Action;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("No InputAction found for Tag: %s in InputConfig"), *Tag.ToString());
	return nullptr;
}
	