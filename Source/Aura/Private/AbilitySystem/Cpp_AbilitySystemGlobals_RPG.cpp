// No Copyright!


#include "AbilitySystem/Cpp_AbilitySystemGlobals_RPG.h"
#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"

FGameplayEffectContext* UCpp_AbilitySystemGlobals_RPG::AllocGameplayEffectContext() const {
	return new FGameplayEffectContext_RPG();
}
