// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cpp_InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCpp_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICpp_InteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

	// = 0 means that this function has to be implemented in the class that inherits from this interface
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetCombatTarget(AActor* Target);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AActor* GetCombatTarget() const;

};
