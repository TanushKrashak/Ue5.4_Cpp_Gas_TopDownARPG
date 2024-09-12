// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Cpp_WGTComp_DamageText.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_WGTComp_DamageText : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================




	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Damage Text")
	void SetDamageText(float DamageAmount, const bool bBlockedHit, const bool bIsCriticalHit);

};
