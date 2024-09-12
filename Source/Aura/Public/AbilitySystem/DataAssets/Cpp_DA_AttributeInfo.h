// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Cpp_DA_AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfo_RPG {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FGameplayTag AttributeTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeDescription = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Info")
	float AttributeValue = 0.0f;
};


UCLASS()
class AURA_API UCpp_DA_AttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROTECTED FUNCTIONS
	//================================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	TArray<FAttributeInfo_RPG> AttributeInformation;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	 FAttributeInfo_RPG FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;


protected:
	//================================================================================================================
	// PROTECTED FUNCTIONS
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================


};
