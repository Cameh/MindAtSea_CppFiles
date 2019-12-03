// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "BTTask_PlaySearchAnim.h"
/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Diver/Diver_AIController.h"

EBTNodeResult::Type UBTTask_PlaySearchAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADiver_AIController* MyController = Cast<ADiver_AIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BlackboardComp = MyController->GetBlackboardComp();

	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		//Searching for player
		BlackboardComp->SetValueAsBool("PlaySearchAnim", true);

		return EBTNodeResult::Succeeded;
	}
}