// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AtHopesBloom.generated.h"

/**
 * 
 */
UCLASS()
class MINDATSEA_API UBTTask_AtHopesBloom : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
