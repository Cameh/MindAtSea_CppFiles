// Made by Cameron Harford. All sound rights go to their respectful creators.

#include "BTTask_DestroyActor.h"
#include "Diver/Diver_AIController.h"

EBTNodeResult::Type UBTTask_DestroyActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Destroy Pawn
	ADiver_AIController* MyController = Cast<ADiver_AIController>(OwnerComp.GetAIOwner());
	MyController->GetPawn()->Destroy();

	return EBTNodeResult::Succeeded;
}