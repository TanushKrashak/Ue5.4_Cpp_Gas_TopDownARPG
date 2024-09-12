// No Copyright!

// Game Includes
#include "Ai/AiC/Cpp_AiC_Master.h"

// Engine Includes
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ACpp_AiC_Master::ACpp_AiC_Master() {
	// Setup the Blackboard Component
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	check(Blackboard);

	// Setup the Behavior Tree Component
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	check(BehaviorTreeComp);
}
