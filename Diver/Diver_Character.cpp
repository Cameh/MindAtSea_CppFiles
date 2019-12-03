// Made by Cameron Harford. All sound rights go to their respectful creators.

#include "Diver_Character.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Base211Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ADiver_Character::ADiver_Character()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	fnEyeSocketRight = TEXT("RightEye");
	fnEyeSocketLeft = TEXT("LeftEye");

	PointLightRight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightRight"));
	PointLightRight->SetupAttachment(GetMesh(), fnEyeSocketRight);
	PointLightRight->SetIntensity(1000.f);
	PointLightRight->SetLightColor(FLinearColor(0.921582f, 1.0f, 0.396755f), true);
	PointLightRight->SetAttenuationRadius(472.283936f);

	PointLightLeft = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightLeft"));
	PointLightLeft->SetupAttachment(GetMesh(), fnEyeSocketLeft);
	PointLightLeft->SetIntensity(1000.f);
	PointLightLeft->SetLightColor(FLinearColor(0.921582f, 1.0f, 0.396755f), true);
	PointLightLeft->SetAttenuationRadius(267.643677f);

	SpotLightRight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightRight"));
	SpotLightRight->SetupAttachment(GetMesh(), fnEyeSocketRight);
	SpotLightRight->SetIntensity(3000.f);
	SpotLightRight->SetLightColor(FLinearColor(1.0f, 0.947307f, 0.412543f), true);
	SpotLightRight->SetAttenuationRadius(1849.789673f);
	SpotLightRight->SetRelativeRotation(FRotator(28.024311, 112.796051, 11.17025));


	SpotLightLeft = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightLeft"));
	SpotLightLeft->SetupAttachment(GetMesh(), fnEyeSocketLeft);
	SpotLightLeft->SetIntensity(3000.f);
	SpotLightLeft->SetLightColor(FLinearColor(1.0f, 0.947307f, 0.412543f), true);
	SpotLightLeft->SetAttenuationRadius(1849.789673f);
	SpotLightLeft->SetRelativeRotation(FRotator(-28.024311, 112.796051, -11.17025));

	//AI

	/*Note: I assign the Controller class in the Blueprint extension of this class
	Because the diver AIController is a blueprint in content and it's better to avoid content references in code.  */

	//The sensing component to detect players by visibility and noise checks.
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 1500;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	SenseTimeOut = 5.f;

	//These values are matched up to the CapsuleComponent above and are used to find navigation paths 
	GetMovementComponent()->NavAgentProps.AgentRadius = 42;
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;

	//Audio
	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("DiverLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	AudioHuntLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("DiverLoopedHuntSoundComp"));
	AudioHuntLoopComp->bAutoActivate = false;
	AudioHuntLoopComp->bAutoDestroy = false;
	AudioHuntLoopComp->SetupAttachment(RootComponent);

	AudioDistanceLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("DiverLoopedDistanceSoundComp"));
	AudioDistanceLoopComp->bAutoActivate = false;
	AudioDistanceLoopComp->bAutoDestroy = false;
	AudioDistanceLoopComp->SetupAttachment(RootComponent);

	AudioChaseThemeLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedChaseThemeSoundComp"));
	AudioChaseThemeLoopComp->bAutoActivate = false;
	AudioChaseThemeLoopComp->bAutoDestroy = false;
	AudioChaseThemeLoopComp->SetupAttachment(RootComponent);

	AudioLostThemeComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LostThemeSoundComp"));
	AudioLostThemeComp->bAutoActivate = false;
	AudioLostThemeComp->bAutoDestroy = false;
	AudioLostThemeComp->SetupAttachment(RootComponent);

	doOnceTorchOn = true;
	doOnceStand = true;
	CurrentPerception = PawnSensingComp->SightRadius;
	
	//Set line trace distance
	TraceDistance = 100.f;

}

// Called when the game starts or when spawned
void ADiver_Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ADiver_Character::OnPlayerCaught);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ADiver_Character::OnHearNoise);
		PawnSensingComp->bEnableSensingUpdates = true;
		
		
	}
}

// Called every frame
void ADiver_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADiver_AIController* AIController = Cast<ADiver_AIController>(GetController());
	ABase211Character* PlayerChar = Cast<ABase211Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

	/*------------- Check for player variables to determine Perception settings -------------*/

	if (PlayerChar != nullptr)
	{
		//If Torch is turned off
		if (PlayerChar->TorchToggle)
		{

			if (doOnceTorchOn)
			{
				CurrentPerception -= 1200.f;
				PawnSensingComp->SightRadius = CurrentPerception;
				doOnceTorchOn = false;
				doOnceTorchOff = true;
			}
		}

				

		//If Torch is turned on
		else if (PlayerChar->TorchToggle == false)
		{
			if (doOnceTorchOff)
			{
				CurrentPerception += 1200.f;
				PawnSensingComp->SightRadius = CurrentPerception;
				doOnceTorchOn = true;
				doOnceTorchOff = false;
			}
		}

		if (PlayerChar->IsCrouching)
		{
			if (!doOnceCrouch)
			{
				CurrentPerception -= 500.f;
				PawnSensingComp->SightRadius = CurrentPerception;
				doOnceCrouch = true;
				doOnceStand = true;
			}
		}

		else if (PlayerChar->IsCrouching == false)
		{
			if (doOnceStand)
			{
				CurrentPerception += 500.f;
				PawnSensingComp->SightRadius = CurrentPerception;
				doOnceCrouch = false;
				doOnceStand = false;
			}
		}
	}

	/*------------- End of Perception Settings -------------*/

	//Check if the last time we sensed a player is beyond the time out value to prevent AI from endlessly following a player. 
	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut
		&& (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
	{
		ADiver_AIController* AIController = Cast<ADiver_AIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			//Reset 
			AIController->SetPlayerCaught(nullptr);
			AIController->LastKnownLocation = PlayerChar->GetActorLocation();
			AIController->RecentlyLostPlayer(true);
			//Stop playing the hunting sound 
			doOncePlayHuntSound = false;
			BroadcastUpdateAudioLoop(false);
		}
	}

	if (bSensedTarget)
	{
		//Setup line trace
			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;

			//Re-initialize hit info
			FHitResult RV_Hit(ForceInit);

			GetWorld()->LineTraceSingleByChannel(
				RV_Hit, //result
				GetCapsuleComponent()->GetComponentLocation(), //start
				GetCapsuleComponent()->GetForwardVector() * TraceDistance + GetCapsuleComponent()->GetComponentLocation(), //end
				ECC_WorldStatic, //collision channel
				RV_TraceParams
			);

			RV_Hit.bBlockingHit; //did hit something? (bool)
			RV_Hit.GetActor(); //the hit actor if there is one
			RV_Hit.ImpactPoint;  //FVector
			RV_Hit.ImpactNormal;  //FVector

			if (RV_Hit.GetActor())
			{
				ABase211Character* PlayerChar = Cast<ABase211Character>(RV_Hit.GetActor());

				//Hit the player
				if (RV_Hit.bBlockingHit && RV_Hit.GetActor() == PlayerChar)
				{
					PlayerChar->Died();
					AudioHuntLoopComp->FadeOut(1.f, 0.f);
					AudioChaseThemeLoopComp->FadeOut(1.f, 0.f);
					AudioDistanceLoopComp->FadeOut(1.f, 0.f);
					bSensedTarget = false;
					AIController->SetPlayerCaught(nullptr);
					doOncePlayHuntSound = false;

				}
			}
			
	}

}

// Called to bind functionality to input
void ADiver_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADiver_Character::GetActorEyesViewPoint(FVector &Location, FRotator &Rotation) const
{
	GetPerceptionLocRot(Location, Rotation);
}

void ADiver_Character::GetPerceptionLocRot_Implementation(FVector &OutLocation, FRotator &OutRotation) const
{
	//Set perception cone location
	OutLocation = GetActorLocation() + FVector(0, 0, 50);
	OutRotation = GetActorRotation();
}

void ADiver_Character::BroadcastUpdateAudioLoop_Implementation(bool bNewSensedTarget)
{
	// Start playing the hunting sound and the "noticed player" sound if the state is about to change 
	if (bNewSensedTarget && bSensedTarget)
	{
		//See player
		AudioLoopComp->FadeOut(1.f, 0.f);
		AudioHuntLoopComp->FadeIn(1.f, 1.f, 0.f);
		AudioDistanceLoopComp->FadeIn(1.f, 1.f, 0.f);
		AudioChaseThemeLoopComp->FadeIn(2.f, 1.f, 0.f);
	}
	else
	{
		//Lost sight of player
		AudioHuntLoopComp->FadeOut(1.f, 0.f);
		AudioDistanceLoopComp->FadeOut(1.f, 0.f);
		AudioChaseThemeLoopComp->FadeOut(1.f, 0.f);
		AudioLoopComp->FadeIn(1.f, 1.f, 0.f);
		AudioLostThemeComp->FadeIn(1.f, 1.f, 0.f);
	}
}

void ADiver_Character::OnPlayerCaught(APawn* Pawn)
{
	//Get a ref to the player controller
	ADiver_AIController* AIController = Cast<ADiver_AIController>(GetController());

	if (AIController)
	{
		
			//Keep track of the time the player was last sensed in order to clear the target 
			LastSeenTime = GetWorld()->GetTimeSeconds();
			bSensedTarget = true;
			AIController->SetPlayerCaught(Pawn);
			if (!doOncePlayHuntSound)
			{
				//Player is being chased
				BroadcastUpdateAudioLoop_Implementation(true);
				doOncePlayHuntSound = true;
			}
		
	
	}

	
}

