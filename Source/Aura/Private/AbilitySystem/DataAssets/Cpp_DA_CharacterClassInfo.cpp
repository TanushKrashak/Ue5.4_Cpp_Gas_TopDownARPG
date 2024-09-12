// No Copyright!


#include "AbilitySystem/DataAssets/Cpp_DA_CharacterClassInfo.h"

FCharacterClassDefaultInfo UCpp_DA_CharacterClassInfo::GetCharacterClassDefaults(ECharacterClass CharacterClass) const {
	return CharacterClassInformation.FindChecked(CharacterClass);
}
