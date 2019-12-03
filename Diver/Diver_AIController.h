// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Diver/TargetPoint/TargetPoint_Actor.h"
#include "Diver_AIController.generated.h"

/**
 * 
 */
UCLASS()
class MINDATSEA_API ADiver_AIController : public AAIController
{
	GENERATED_BODY()
	
	ADiver_AIController(const class FObjectInitializer& ObjectInitializer);

	//Called whenever the controller possesses a character bot
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName BotTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName MoveLocationName;	

	

	

public:



	//Returns BehaviorComp subobject
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	class APawn* DiverPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<ATargetPoint_Actor*> TargetPoints;

	//Allows pawn destroy after moving to target point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool DestroyAfterMove;

	//Last known player location to search
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector LastKnownLocation;

	bool bPathSuccessful;

	void SetPlayerCaught(APawn* Pawn);

	void RecentlyLostPlayer(bool bLostPlayer);

	bool PathingCheck();

};
	 