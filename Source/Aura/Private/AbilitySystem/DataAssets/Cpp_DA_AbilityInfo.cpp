// No Copyright!

// Game Includes
#include "AbilitySystem/DataAssets/Cpp_DA_AbilityInfo.h"
#include "Aura/RpgLogChannels.h"

FAbilityInfo_RPG UCpp_DA_AbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound /*= false*/) const {
	for (const FAbilityInfo_RPG& AbilityInfo : AllAbilityInfo) {
		if (AbilityInfo.AbilityTag.MatchesTag(AbilityTag)) {
			return AbilityInfo;
		}
	}
	if (bLogNotFound) {
		UE_LOG(LogRpg, Error, TEXT("Ability Info with Tag %s not found in %s"), *AbilityTag.ToString(), *GetNameSafe(this));
	}	
	return FAbilityInfo_RPG();
}
