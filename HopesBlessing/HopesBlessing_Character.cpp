// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "HopesBlessing_Character.h"

// Sets default values
AHopesBlessing_Character::AHopesBlessing_Character()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Plant1StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant1StaticMeshComponent"));
	Plant1StaticMesh->SetupAttachment(RootComponent);
	Plant1StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	Plant2StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant2StaticMeshComponent"));
	Plant2StaticMesh->SetupAttachment(RootComponent);
	Plant2StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	BallStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallStaticMeshComponent"));
	BallStaticMesh->SetupAttachment(RootComponent);
	BallStaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	FinishParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FinishParticleSystem"));

	ColourLerp = 0.f;
	StartMatColour = (FLinearColor(1.f, 0.08f, 0.f));
	FinishMatColour = (FLinearColor(0.28f, 1.f, 0.68f));

	StartScale = FVector(0.25f, 0.25f, 0.25f);
	FinishScale = FVector(0.3f, 0.3f, 0.3f);

	ShrinkScale = FVector(0.01f, 0.01f, 0.01f);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightRight"));
	PointLight->SetIntensity(15000.f);
	PointLight->SetLightColor(FLinearColor(1.f, 0.08f, 0.f), true);
	PointLight->SetAttenuationRadius(960.f);
	PointLight->SetupAttachment(RootComponent);
	PointLight->RelativeLocation = FVector(0.f, 0.f, 0.f);

	/* These values are matched up to the CapsuleComponent above and are used to find navigation paths */
	GetMovementComponent()->NavAgentProps.AgentRadius = 500;
	GetMovementComponent()->NavAgentProps.AgentHeight = 500;


	//Sounds
	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	AudioCompleteComp = CreateDefaultSubobject<UAudioComponent>(TEXT("CompletedSoundComp"));
	AudioCompleteComp->bAutoActivate = false;
	AudioCompleteComp->bAutoDestroy = false;
	AudioCompleteComp->SetupAttachment(RootComponent);

	AudioHoverLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("HoverLoopSoundComp"));
	AudioHoverLoopComp->bAutoActivate = false;
	AudioHoverLoopComp->bAutoDestroy = false;
	AudioHoverLoopComp->SetupAttachment(RootComponent);

	AudioBloomFoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BloomFoundAudioComp"));
	AudioBloomFoundComp->bAutoActivate = false;
	AudioBloomFoundComp->bAutoDestroy = false;
	AudioBloomFoundComp->SetupAttachment(RootComponent);

	OrbParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OrbParticleComponent"));
	OrbParticle->SetupAttachment(RootComponent);
	OrbParticle->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AHopesBlessing_Character::BeginPlay()
{
	Super::BeginPlay();
	
	//Used for shake
	OriginalLoc = GetActorLocation();

	//Move to location once charged
	MoveToAILoc = GetActorLocation().Z - 80.f;
}

// Called every frame
void AHopesBlessing_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerChar)
	{
		//Change colour to Blue
		if (PlayerChar->bLookingAtActor && !bCompletedCharge)
		{
			SmoothColourChange(FinishMatColour, DeltaTime, false, 4.5f);
		}

		//Change colour to Red
		else if (!PlayerChar->bLookingAtActor && !bCompletedCharge)
		{
			SmoothColourChange(StartMatColour, DeltaTime, true, 1.f);
		}

		//Start shake
		if (!bShakeOnce && PointLight->GetLightColor().B > StartMatColour.B)
		{
			GetWorldTimerManager().SetTimer(ShakeTimerHandle, this, &AHopesBlessing_Character::Shake, 0.01f, true, 0.f);
			bShakeOnce = true;
			bCheckShakeOnce = true;
		}

		//End shake
		else if (bCheckShakeOnce && PointLight->GetLightColor().B == 0.f)
		{
			GetWorldTimerManager().ClearTimer(ShakeTimerHandle);
			AudioLoopComp->FadeOut(0.05f, 0.f);
			PlayerChar = nullptr;
			bShakeOnce = false;
			bCheckShakeOnce = false;

		}

		//Start scale
		if (bCompletedCharge && !bScaleOnce)
		{
			//ShrinkTimerDel.BindUFunction(this, FName("ShrinkToAISetup"));
			//GetWorldTimerManager().SetTimer(ShrinkTimerHandle, ShrinkTimerDel, 0.f, false);
			ShrinkToAISetup();
		}

		if (bIsShrink && !bShrinkOnce)
		{
			ShrinkToAI(ShrinkScale, DeltaTime);
		}

	}

	
}

// Called to bind functionality to input
void AHopesBlessing_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHopesBlessing_Character::ChangeColourSwitch()
{
	//Enable colour change
	bColourChange = true;
	PlayerChar = Cast<ABase211Character>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!AudioLoopComp->IsPlaying() && !bCompletedCharge)
	{
		AudioLoopComp->FadeIn(0.1f, 1.f, 0.f);
	}
}

void AHopesBlessing_Character::SmoothColourChange(FLinearColor FinishColour, float DeltaSeconds, bool bReverse, float InterpSpeed)
{


	if (PointLight->GetLightColor().R != (FinishColour.R) || PointLight->GetLightColor().G != (FinishColour.G) || PointLight->GetLightColor().B != (FinishColour.B))
	{
		//Change colour over time
		PointLight->SetLightColor(FLinearColor(FMath::FInterpConstantTo(PointLight->GetLightColor().R, FinishColour.R, DeltaSeconds, 1.f), FMath::FInterpConstantTo(PointLight->GetLightColor().G, FinishColour.G, DeltaSeconds, 2.f), FMath::FInterpConstantTo(PointLight->GetLightColor().B, FinishColour.B, DeltaSeconds, 1.f)), true);
		AudioLoopComp->SetFloatParameter("Pitch", PointLight->GetLightColor().G);
	}

	//Make complete if the right colour has been reached (with some leeway)
	if (PointLight->GetLightColor().R >= (FinishColour.R - 0.001f) && PointLight->GetLightColor().G >= (FinishColour.G - 0.001f) && PointLight->GetLightColor().B >= (FinishColour.B - 0.001f) && !bReverse)
	{
		if (!bCompletedCharge)
		{
			ChargedUp();
			GetWorldTimerManager().ClearTimer(ShakeTimerHandle);
		}
	}


}

void AHopesBlessing_Character::ChargedUp()
{
	//Spawn particle effect
	OrbParticle->Activate();
	AudioLoopComp->FadeOut(0.1f, 0.f);
	AudioCompleteComp->FadeIn(0.1f, 1.f, 0.f);
	PlayerChar->HopesBlessingShakeEvent();
	bCompletedCharge = true;

}

void AHopesBlessing_Character::Shake()
{
	//Shake with colour intensity
	SetActorLocation(FVector(OriginalLoc.X + ((FMath::RandRange(-PointLight->GetLightColor().B, PointLight->GetLightColor().B) * 4)), OriginalLoc.Y + ((FMath::RandRange(-PointLight->GetLightColor().B, PointLight->GetLightColor().B) * 4)), OriginalLoc.Z + ((FMath::RandRange(-PointLight->GetLightColor().B, PointLight->GetLightColor().B) * 4))));

}

void AHopesBlessing_Character::ScaleChange(FVector Scale, float DeltaSeconds)
{
	//Scale up with the colour change

	//Scale back down
	if (GetActorScale3D().X >= (FinishScale.X - 0.02f))
	{
		SetActorScale3D(FVector(FMath::FInterpConstantTo(GetActorScale3D().X, StartScale.X, DeltaSeconds, 1.f), FMath::FInterpConstantTo(GetActorScale3D().Y, StartScale.Y, DeltaSeconds, 1.f), FMath::FInterpConstantTo(GetActorScale3D().Z, StartScale.Z, DeltaSeconds, 1.f)));
		bScaledBig = true;
	}
	//Scale Up
	else if (GetActorScale3D().X >= StartScale.X && GetActorScale3D().Y >= StartScale.Y && GetActorScale3D().Z >= StartScale.Z && GetActorScale3D() != FinishScale && !bScaledBig)
	{
		SetActorScale3D(FVector(FMath::FInterpConstantTo(GetActorScale3D().X, Scale.X, DeltaSeconds, 2.f), FMath::FInterpConstantTo(GetActorScale3D().Y, Scale.Y, DeltaSeconds, 2.f), FMath::FInterpConstantTo(GetActorScale3D().Z, Scale.Z, DeltaSeconds, 2.f)));

	}

	//Do once
	else if (GetActorScale3D().X <= (StartScale.X + 0.05) && bScaledBig)
	{
		//Binding the function with specific variables
		ShrinkTimerDel.BindUFunction(this, FName("ShrinkToAISetup"));
		GetWorldTimerManager().SetTimer(ShrinkTimerHandle, ShrinkTimerDel, 2.f, false);
		bScaleOnce = true;
	}

}

void AHopesBlessing_Character::ShrinkToAISetup()
{
	bIsShrink = true;
	bScaleOnce = true;
}

void AHopesBlessing_Character::ShrinkToAI(FVector Scale, float DeltaSeconds)
{
	//Shrink to a tiny size and then get AI setup
	if (GetActorScale3D().X > 0.013f)
	{
		SetActorScale3D(FVector(FMath::FInterpTo(GetActorScale3D().X, Scale.X, DeltaSeconds, 1.f), FMath::FInterpTo(GetActorScale3D().Y, Scale.Y, DeltaSeconds, 1.f), FMath::FInterpTo(GetActorScale3D().Z, Scale.Z, DeltaSeconds, 1.f)));
		PointLight->SetIntensity(FMath::FInterpTo(PointLight->Intensity, 50.f, DeltaSeconds, 10.f));
		OrbParticle->SetRelativeScale3D(FVector(FMath::FInterpTo(OrbParticle->RelativeScale3D.X, 15.f, DeltaSeconds, 5.f), FMath::FInterpTo(OrbParticle->RelativeScale3D.Y, 15.f, DeltaSeconds, 5.f), FMath::FInterpTo(OrbParticle->RelativeScale3D.Z, 15.f, DeltaSeconds, 5.f)));
		SetActorLocation(FVector((GetActorLocation().X), (GetActorLocation().Y), (FMath::FInterpTo(GetActorLocation().Z, MoveToAILoc, DeltaSeconds, 1.f))));
	}

	else
	{
		AHopesBlessing_AIController* AIController = Cast<AHopesBlessing_AIController>(GetController());

		Plant1StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Plant2StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		BallStaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		AIController->SetReadyToRoam();
		AudioHoverLoopComp->FadeIn(0.5f, 1.f);
		bShrinkOnce = true;
	}
}
