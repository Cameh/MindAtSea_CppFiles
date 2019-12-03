// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "BTTask_AtHopesBloom.h"
#include "HopesBlessing_AIController.h"
#include "HopesBloom/HopesBloom_Actor.h"

EBTNodeResult::Type UBTTask_AtHopesBloom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Add one to the Bloom's count
	AHopesBlessing_AIController* MyController = Cast<AHopesBlessing_AIController>(OwnerComp.GetAIOwner());
	MyController->IncrementBloom();
	MyController->GetPawn()->Destroy();

	return EBTNodeResult::Succeeded;
}