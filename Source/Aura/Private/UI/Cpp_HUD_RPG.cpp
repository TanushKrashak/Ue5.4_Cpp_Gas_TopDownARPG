// No Copyright!


#include "UI/Cpp_HUD_RPG.h"
#include "UI/Widgets/Cpp_WGT_Master.h"
#include "UI/Cpp_WidgetController_HUD.h"
#include "UI/WidgetControllers/Cpp_WidgetController_Attributes.h"
#include "UI/WidgetControllers/Cpp_WidgetController_Abilities.h"


UCpp_WidgetController_HUD* ACpp_HUD_RPG::GetHUDWidgetController(const FWidgetControllerParams& Params) {
	if (HUDWidgetController == nullptr) {
		HUDWidgetController = NewObject<UCpp_WidgetController_HUD>(this, HUDWidgetControllerClass);
		HUDWidgetController->SetWidgetControllerParams(Params);
		// Let's the controller update all of the attributes
		HUDWidgetController->BindCallbacksToDependancies();
	}
	return HUDWidgetController;
}
UCpp_WidgetController_Attributes* ACpp_HUD_RPG::GetAttributeWidgetController(const FWidgetControllerParams& Params) {
	if (AttributeWidgetController == nullptr) {
		AttributeWidgetController = NewObject<UCpp_WidgetController_Attributes>(this, AttributeWidgetControllerClass);
		AttributeWidgetController->SetWidgetControllerParams(Params);
		// Let's the controller update all of the attributes
		AttributeWidgetController->BindCallbacksToDependancies();
	}
	return AttributeWidgetController;
}
UCpp_WidgetController_Abilities* ACpp_HUD_RPG::GetAbilitiesWidgetController(const FWidgetControllerParams& Params) {
	if (AbilitiesWidgetController == nullptr) {
		AbilitiesWidgetController = NewObject<UCpp_WidgetController_Abilities>(this, AbilitiesWidgetControllerClass);
		AbilitiesWidgetController->SetWidgetControllerParams(Params);
		// Let's the controller update all of the attributes
		AbilitiesWidgetController->BindCallbacksToDependancies();
	}
	return AbilitiesWidgetController;
}

void ACpp_HUD_RPG::InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* ASet) {
	// Required checks
	checkf(HUDWidgetClass, TEXT("HUD Widget Class Not Set! CHECK HUD CLASS"));
	checkf(HUDWidgetControllerClass, TEXT("HUD Widget Controller Class Not Set! CHECK HUD CLASS"));
	
	HUDWidget = CreateWidget<UCpp_WGT_Master>(GetWorld(), HUDWidgetClass);
	if (HUDWidget) {
		// Create Widget Controller and set it to the Widget's Controller
		const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, ASet);
		UCpp_WidgetController_HUD* WidgetController = GetHUDWidgetController(WidgetControllerParams);
		HUDWidget->SetWidgetController(HUDWidgetController);
		// This allows the Widget to get the initial values from the AttributeSet only after the HUD is created
		WidgetController->BroadcastInitialValues();

		HUDWidget->AddToViewport();
	}
}


