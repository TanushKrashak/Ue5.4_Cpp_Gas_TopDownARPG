// No Copyright!

// Game Includes
#include "UI/WidgetControllers/Cpp_WidgetController_Attributes.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "AbilitySystem/DataAssets/Cpp_DA_AttributeInfo.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Core/Cpp_PS_Player.h"


void UCpp_WidgetController_Attributes::BindCallbacksToDependancies() {		

	// Goes through all the attributes in the AttributeSet and add a lambda to the delegate to broadcast the changes
	for (auto& Pair : GetCastedASet()->TagToAttributes) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).
			AddLambda( [this, Pair] (const FOnAttributeChangeData& Data) {
				BroadcastAttributeInfo(Pair.Key,(const FGameplayAttribute&)Pair.Value());
			}
		);
	}

	// Bind to Attribute Points Changed
	GetCastedPS()->OnAttributePointsChanged.AddLambda(
		[this](int32 NewAttributePoints) {
			OnAttributePointsChanged.Broadcast(NewAttributePoints);
		}
	);
	

}
void UCpp_WidgetController_Attributes::BroadcastInitialValues() {
	check(AttributeInfo);

	// Broadcast all the attributes
	for (auto& Pair : GetCastedASet()->TagToAttributes) {
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}		
	// Broadcast the Attribute Points
	OnAttributePointsChanged.Broadcast(GetCastedPS()->GetAttributePoints());	
}

void UCpp_WidgetController_Attributes::UpgradeAttribute(const FGameplayTag& AttributeTag) {
	GetCastedASC()->UpgradeAttribute(AttributeTag);
}

void UCpp_WidgetController_Attributes::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
															  const FGameplayAttribute& Attribute) const {
	FAttributeInfo_RPG TempInfo = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	TempInfo.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangedDelegate.Broadcast(TempInfo);
}
