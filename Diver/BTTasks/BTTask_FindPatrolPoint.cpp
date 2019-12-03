// Made by Cameron Harford. All sound rights go to their respectful creators.

#include "BTTask_FindPatrolPoint.h"
/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Diver/Diver_AIController.h"
#include "Diver/TargetPoint/TargetPoint_Actor.h"
#include "TimerManager.h"
#include "Base211Character.h"
#include "Diver/Diver_Character.h"

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADiver_AIController* MyController = Cast<ADiver_AIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BlackboardComp = MyController->GetBlackboardComp();
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		//Only set a random Target Point if more than 0 points exist
		if (MyController->TargetPoints.Num() > 0)
		{
			BlackboardComp->SetValueAsObject("TargetActor", MyController->TargetPoints[rand() % MyController->TargetPoints.Num()]);

			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
}

