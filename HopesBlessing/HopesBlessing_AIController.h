// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "HopesBloom/HopesBloom_Actor.h"
#include "HopesBlessing_AIController.generated.h"

/**
 * 
 */
UCLASS()
class MINDATSEA_API AHopesBlessing_AIController : public AAIController
{
	GENERATED_BODY()
	
	AHopesBlessing_AIController(const class FObjectInitializer& ObjectInitializer);

	//Called whenever the controller possesses a character bot
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

public:

	//Returns BehaviorComp subobject
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	class APawn* HopesBlessingPawn;

	AHopesBloom_Actor* HopesBloomRef;

	//When charged this is set to true
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Follow)
	bool bCanFollow;

	void SetReadyToRoam();

	void SetBloomActor(AActor* BloomActor);

	void SetFollow();

	void IncrementBloom();
};
