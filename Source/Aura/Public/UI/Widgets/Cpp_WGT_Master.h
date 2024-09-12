// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_Master.generated.h"


UCLASS()
class AURA_API UCpp_WGT_Master : public UUserWidget
{
	GENERATED_BODY()
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UObject> WidgetController;


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetController(UObject* Controller);

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================


	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

};
