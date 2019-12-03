// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "HopesBlessing_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HopesBlessing_Character.h"
#include "Base211Character.h"

AHopesBlessing_AIController::AHopesBlessing_AIController(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	//Initializes PlayerState
	bWantsPlayerState = true;


}

void AHopesBlessing_AIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	HopesBlessingPawn = InPawn;

	AHopesBlessing_Character* HopesBlessingBot = Cast<AHopesBlessing_Character>(InPawn);
	if (HopesBlessingBot)
	{
		if (HopesBlessingBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*HopesBlessingBot->BehaviorTree->BlackboardAsset);

		}
		BehaviorComp->StartTree(*HopesBlessingBot->BehaviorTree);
	}
}

void AHopesBlessing_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	//Stop any behavior running as we no longer have a pawn to control
	BehaviorComp->StopTree();
}

void AHopesBlessing_AIController::SetReadyToRoam()
{
	ABase211Character* PlayerRef = Cast<ABase211Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(PlayerRef)
	{ 
		//If player has more than 0 Blessings already following, set the follow object to the last Blessing in the array
		if ((*PlayerRef).HopesBlessingArray.Num() > 0)
		{
			BlackboardComp->SetValueAsObject("FollowRef", (*PlayerRef).HopesBlessingArray.Last());
			BlackboardComp->SetValueAsObject("PlayerRef", PlayerRef);
			(*PlayerRef).HopesBlessingArray.Add(HopesBlessingPawn);
		}
		//Otherwise set the follow target as the player
		else
		{
			BlackboardComp->SetValueAsObject("PlayerRef", PlayerRef);
			(*PlayerRef).HopesBlessingArray.Add(HopesBlessingPawn);
		}
		
	}
	
	BlackboardComp->SetValueAsBool("CanFollow", true);
}

void AHopesBlessing_AIController::SetBloomActor(AActor* BloomActor)
{
	ABase211Character* PlayerRef = Cast<ABase211Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
	HopesBloomRef = Cast<AHopesBloom_Actor>(BloomActor);
	if (PlayerRef)
	{
		if ((*PlayerRef).HopesBlessingArray.IsValidIndex(0))
		{
			//Remove Blessing from array and set to move to Bloom
			BlackboardComp->SetValueAsObject("HopesBloomActor", BloomActor);
			AHopesBlessing_Character* PawnRef = Cast<AHopesBlessing_Character>(GetPawn());
			PawnRef->AudioBloomFoundComp->FadeIn(0.01f, 1.f);
			(*PlayerRef).HopesBlessingArray.RemoveSingle(HopesBlessingPawn);
			if ((*PlayerRef).HopesBlessingArray.IsValidIndex(0))
			{
				//Set newly shuffled Blessing to follow player instead of the Blessing that just left the array
				AHopesBlessing_AIController* Element0Controller = Cast<AHopesBlessing_AIController>((*PlayerRef).HopesBlessingArray[0]->GetController());
				Element0Controller->SetFollow();
			}
		}
	}
}

void AHopesBlessing_AIController::SetFollow()
{
	BlackboardComp->SetValueAsObject("FollowRef", nullptr);
}

void AHopesBlessing_AIController::IncrementBloom()
{
	//Increments Bloom's Blessing count
	HopesBloomRef->IncrementBlessingCheck();
}