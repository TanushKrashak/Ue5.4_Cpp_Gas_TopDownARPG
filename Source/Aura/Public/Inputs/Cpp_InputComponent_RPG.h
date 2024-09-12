// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Cpp_DA_InputConfig.h"
#include "Cpp_InputComponent_RPG.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCpp_InputComponent_RPG : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UCpp_DA_InputConfig* InputConfig, UserClass* Object, 
							PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, 
							HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UCpp_InputComponent_RPG::BindAbilityActions(const UCpp_DA_InputConfig* InputConfig, UserClass* Object,
												 PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, 
												 HeldFuncType HeldFunc)  {	
	check(InputConfig);

	// Bind all actions with valid tags
	for (const FRPGInputAction& Action : InputConfig->AbilityInputActions) {
		if (Action.Action && Action.InputTag.IsValid()) {
			if (PressedFunc) {
				BindAction(Action.Action, ETriggerEvent::Started, Object, PressedFunc,	Action.InputTag);
			}
			if (ReleasedFunc) {
				BindAction(Action.Action, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc) {
				BindAction(Action.Action, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
