// No Copyright!


#include "AbilitySystem/DataAssets/Cpp_DA_AttributeInfo.h"
#include "Aura/RpgLogChannels.h"

FAttributeInfo_RPG UCpp_DA_AttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound /*= false*/) const {
	for (const auto& AttributeInfo : AttributeInformation) {
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag)) {
			return AttributeInfo;
		}
	}
	if (bLogNotFound) {
		UE_LOG(LogRpg, Error, TEXT("Attribute Info for tag %s not found in %s"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAttributeInfo_RPG();
}
