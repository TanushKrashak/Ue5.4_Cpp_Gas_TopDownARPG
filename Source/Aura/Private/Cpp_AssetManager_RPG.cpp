// No Copyright!


#include "Cpp_AssetManager_RPG.h"
#include "Cpp_AllGameplayTags.h"
#include "AbilitySystemGlobals.h"

UCpp_AssetManager_RPG& UCpp_AssetManager_RPG::Get() {
	check(GEngine);

	UCpp_AssetManager_RPG* RpgAssetManager = Cast<UCpp_AssetManager_RPG>(GEngine->AssetManager);
	return *RpgAssetManager;
}

void UCpp_AssetManager_RPG::StartInitialLoading() {
	Super::StartInitialLoading();
	
	FAllGameplayTags::InitializeNativeGameplayTags();

	// this is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
