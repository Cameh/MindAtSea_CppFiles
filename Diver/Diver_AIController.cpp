// Made by Cameron Harford. All sound rights go to their respectful creators.

#include "Diver_AIController.h"
#include "Diver_Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Base211Character.h"


ADiver_AIController::ADiver_AIController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	//Match variables with the AI/Blackboard 
	BotTypeKeyName = "BotType";
	TargetEnemyKeyName = "TargetEnemy";
	MoveLocationName = "MoveLocation";



	//Initializes PlayerState so we can assign a team index to AI
	bWantsPlayerState = true;

	
}

void ADiver_AIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	DiverPawn = InPawn;

	ADiver_Character* DiverBot = Cast<ADiver_Character>(InPawn);
	if (DiverBot)
	{
		if (DiverBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*DiverBot->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*DiverBot->BehaviorTree);
	}
}


void ADiver_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	//Stop any behavior running as I no longer have a pawn to control
	BehaviorComp->StopTree();
}



void ADiver_AIController::SetPlayerCaught(APawn* Pawn)
{
	if (BlackboardComp)
	{
		if (BlackboardComp->GetValueAsBool("RecentlyLost") == false)
		{
			//Set TargetPlayer and allow the AI to chase player
			BlackboardComp->SetValueAsObject("TargetPlayer", Pawn);
			RecentlyLostPlayer(false);
			BlackboardComp->SetValueAsBool("PlayerSpotted", true);
		}
		else
		{
			BlackboardComp->SetValueAsBool("PlayerSpotted", false);
		}
	}
}

void ADiver_AIController::RecentlyLostPlayer(bool bLostPlayer)
{
	//Set whether the player has been lost recently
	BlackboardComp->SetValueAsBool("RecentlyLost", bLostPlayer);
}

bool ADiver_AIController::PathingCheck()
{
	//If path exists then it will move there
	bPathSuccessful = BlackboardComp->GetValueAsBool("Path Exists");

	return bPathSuccessful;

}