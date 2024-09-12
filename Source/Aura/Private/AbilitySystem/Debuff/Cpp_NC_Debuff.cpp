// No Copyright!

// Game Includes
#include "AbilitySystem/Debuff/Cpp_NC_Debuff.h"
#include "Interfaces/Cpp_CombatInterface.h"

// Engine Includes
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UCpp_NC_Debuff::UCpp_NC_Debuff() {
	bAutoActivate = false;
}

void UCpp_NC_Debuff::BeginPlay() {
	Super::BeginPlay();

	ICpp_CombatInterface* CombatInterface = Cast<ICpp_CombatInterface>(GetOwner());

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC) {
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCpp_NC_Debuff::DebuffTagChanged);
	}
	else if (CombatInterface) {
		CombatInterface->GetOnAscRegistredDelegate().AddWeakLambda(this, [this]
		(UAbilitySystemComponent* ASC) {
			ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCpp_NC_Debuff::DebuffTagChanged);
			}
		);
	}
	if (CombatInterface) {
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UCpp_NC_Debuff::OnOwnerDeath);
	}
}

void UCpp_NC_Debuff::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount) {
	bool bOwnerValid = IsValid(GetOwner());
	bool bOwnerImplementsCombatInterface = bOwnerValid && GetOwner()->Implements<UCpp_CombatInterface>();
	bool bOwnerDead = bOwnerImplementsCombatInterface && ICpp_CombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && !bOwnerDead) {
		Activate();
	}
	else {
		Deactivate();
	}
}

void UCpp_NC_Debuff::OnOwnerDeath(AActor* DeadActor) {
	Deactivate();
}