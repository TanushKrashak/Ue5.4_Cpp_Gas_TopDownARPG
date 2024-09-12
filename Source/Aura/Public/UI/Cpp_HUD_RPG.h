// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Cpp_HUD_RPG.generated.h"

#pragma region Forward Declarations
class UCpp_WGT_Master;
class UCpp_WidgetController_HUD;
class UCpp_WidgetController_Attributes;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;
class APlayerController;
class UCpp_WidgetController_Abilities;
#pragma endregion

UCLASS()
class AURA_API ACpp_HUD_RPG : public AHUD
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================

	// Getters for the Widget Controllers
	UCpp_WidgetController_HUD* GetHUDWidgetController(const FWidgetControllerParams& Params);
	UCpp_WidgetController_Attributes* GetAttributeWidgetController(const FWidgetControllerParams& Params);
	UCpp_WidgetController_Abilities* GetAbilitiesWidgetController(const FWidgetControllerParams& Params);
		
	

	void InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* ASet);

protected:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// HUD Widget
	UPROPERTY()
	TObjectPtr<UCpp_WGT_Master> HUDWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCpp_WGT_Master> HUDWidgetClass;
	// HUD Widget Controller
	UPROPERTY()
	TObjectPtr<UCpp_WidgetController_HUD> HUDWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UCpp_WidgetController_HUD> HUDWidgetControllerClass;

	// Attribute Widget Controller
	UPROPERTY()
	TObjectPtr<UCpp_WidgetController_Attributes> AttributeWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UCpp_WidgetController_Attributes> AttributeWidgetControllerClass;

	// Abilities Widget Controller
	UPROPERTY()
	TObjectPtr<UCpp_WidgetController_Abilities> AbilitiesWidgetController;
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UCpp_WidgetController_Abilities> AbilitiesWidgetControllerClass;

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================


};
