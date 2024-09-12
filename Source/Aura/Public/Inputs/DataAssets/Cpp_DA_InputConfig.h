// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Cpp_DA_InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FRPGInputAction {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* Action = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;
};

UCLASS()
class AURA_API UCpp_DA_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRPGInputAction> AbilityInputActions;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& Tag) const;
};
