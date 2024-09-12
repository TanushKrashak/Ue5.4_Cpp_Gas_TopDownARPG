// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cpp_GM_RPG.generated.h"

// Forward Declarations
class UCpp_DA_CharacterClassInfo;
class UCpp_DA_AbilityInfo;

UCLASS()
class AURA_API ACpp_GM_RPG : public AGameModeBase
{
	GENERATED_BODY()

public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCpp_DA_CharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Information")
	TObjectPtr<UCpp_DA_AbilityInfo> AbilityInfo;
	
};
