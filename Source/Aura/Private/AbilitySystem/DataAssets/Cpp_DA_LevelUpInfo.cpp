// No Copyright!


#include "AbilitySystem/DataAssets/Cpp_DA_LevelUpInfo.h"

int32 UCpp_DA_LevelUpInfo::FindLevelForExperience(int32 Experience) const {

	// figure out which level the player is at based on the experience, experience is cumulative so its already the total experience
	for (int32 i = 1; i < LevelUpInfo.Num(); i++) {
		if (Experience < LevelUpInfo[i].ExperienceRequired) {
			return i;
		}
	}
	return 1;	
}
