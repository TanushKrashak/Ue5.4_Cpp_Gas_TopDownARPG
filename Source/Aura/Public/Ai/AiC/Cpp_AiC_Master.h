// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Cpp_AiC_Master.generated.h"

// Forward Declarations
class UBlackboardComponent;
class UBehaviorTreeComponent;


UCLASS()
class AURA_API ACpp_AiC_Master : public AAIController
{
	GENERATED_BODY()
	
public:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================



	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================
	ACpp_AiC_Master();

protected:
	//===============================================================================================================
	// PROPERTIES & VARIABLES
	//===============================================================================================================
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;

	//===============================================================================================================
	// FUNCTIONS
	//===============================================================================================================


};
