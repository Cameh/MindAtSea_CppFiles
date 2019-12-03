// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "HopesBloom_Actor.h"
#include "HopesBlessing/HopesBlessing_AIController.h"

// Sets default values
AHopesBloom_Actor::AHopesBloom_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneGroup = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneGroup;

	//Create all static meshes
	Plant1StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant1StaticMeshComponent"));
	Plant1StaticMesh->SetupAttachment(RootComponent);
	Plant1StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	Plant2StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant2StaticMeshComponent"));
	Plant2StaticMesh->SetupAttachment(RootComponent);
	Plant2StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	Plant3StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant3StaticMeshComponent"));
	Plant3StaticMesh->SetupAttachment(RootComponent);
	Plant3StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	Plant4StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plant4StaticMeshComponent"));
	Plant4StaticMesh->SetupAttachment(RootComponent);
	Plant4StaticMesh->RelativeLocation = FVector(0.f, 0.f, 0.f);

	//Create collision box
	AcceptanceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AcceptanceBoxComponent"));
	AcceptanceBox->SetupAttachment(RootComponent);
	AcceptanceBox->RelativeLocation = FVector(0.f, 0.f, 0.f);

	RestoredParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RestoredParticleComponent"));
	RestoredParticle->SetupAttachment(RootComponent);
	RestoredParticle->bAutoActivate = false;

	GodRayParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GodRayParticleComponent"));
	GodRayParticle->SetupAttachment(RootComponent);
	GodRayParticle->bAutoActivate = true;

	//Create audio components
	AudioCreakComp = CreateDefaultSubobject<UAudioComponent>(TEXT("CreakSoundComp"));
	AudioCreakComp->bAutoActivate = true;
	AudioCreakComp->bAutoDestroy = false;
	AudioCreakComp->SetupAttachment(RootComponent);

	AudioShoreComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ShoreSoundComp"));
	AudioShoreComp->bAutoActivate = false;
	AudioShoreComp->bAutoDestroy = false;
	AudioShoreComp->SetupAttachment(RootComponent);

	PointLightBlue = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightBlue"));
	PointLightBlue->SetupAttachment(RootComponent);
	PointLightBlue->SetIntensity(8000.f);
	PointLightBlue->SetLightColor(FLinearColor(0.366253f, 1.0f, 0.991102f), true);
	PointLightBlue->SetAttenuationRadius(480.f);

	BlessingAmount = 0;
}

// Called when the game starts or when spawned
void AHopesBloom_Actor::BeginPlay()
{
	Super::BeginPlay();

	AcceptanceBox->OnComponentBeginOverlap.AddDynamic(this, &AHopesBloom_Actor::OnOverlap);

	//Make materials dynamic instances

	UMaterialInterface * Plant1Material = Plant1StaticMesh->GetMaterial(0);

	Plant1MatInstance = Plant1StaticMesh->CreateDynamicMaterialInstance(0, Plant1Material);

	UMaterialInterface* Plant2Material = Plant2StaticMesh->GetMaterial(0);

	Plant2MatInstance = Plant2StaticMesh->CreateDynamicMaterialInstance(0, Plant2Material);

	UMaterialInterface* Plant3Material = Plant3StaticMesh->GetMaterial(0);

	Plant3MatInstance = Plant3StaticMesh->CreateDynamicMaterialInstance(0, Plant3Material);

	UMaterialInterface* Plant4Material = Plant4StaticMesh->GetMaterial(0);

	Plant4MatInstance = Plant4StaticMesh->CreateDynamicMaterialInstance(0, Plant4Material);

	BlessingAmountOriginal = BlessingAmount;

}

// Called every frame
void AHopesBloom_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHopeRestore && !bHopeRestoreComplete)
	{
		RestoreHope(DeltaTime);
	}
}

void AHopesBloom_Actor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* WorldRef = GetWorld();
	ABase211Character* CharacterRef = Cast<ABase211Character>(WorldRef->GetFirstPlayerController()->GetCharacter());
	if (CharacterRef)
	{
		if (OtherActor == CharacterRef)
		{
			if ((*CharacterRef).HopesBlessingArray.Num() >= BlessingAmount)
			{
				//Leave out the increment for i and instead have it further down
				for (int i = 0; i < BlessingAmount;)
				{
					if ((*CharacterRef).HopesBlessingArray.IsValidIndex(i))
					{
						AHopesBlessing_AIController* PawnController = Cast<AHopesBlessing_AIController>((*CharacterRef).HopesBlessingArray[i]->GetController());
						if (PawnController)
						{
							PawnController->SetBloomActor(this);

							//Decrement Blessing Amount instead of incrementing i to keep the array check at index 0
							BlessingAmount--;
						}

					}
					else
					{
						//Increment if there is no valid index (this isn't ever called but kept in to avoid the case where it might be needed)
						i++;
					}

				}


			}

		}
	}

}

void AHopesBloom_Actor::RestoreHope(float DeltaTime)
{
	if (Plant1MatInstance->K2_GetScalarParameterValue("Restoration") != 0 && Plant2MatInstance->K2_GetScalarParameterValue("Restoration") != 0 && Plant3MatInstance->K2_GetScalarParameterValue("Restoration") != 0 && Plant4MatInstance->K2_GetScalarParameterValue("Restoration") != 0)
	{
		Plant1MatInstance->SetScalarParameterValue("Restoration", FMath::FInterpConstantTo(Plant1MatInstance->K2_GetScalarParameterValue("Restoration"), 0, DeltaTime, 1.f));
		Plant2MatInstance->SetScalarParameterValue("Restoration", FMath::FInterpConstantTo(Plant2MatInstance->K2_GetScalarParameterValue("Restoration"), 0, DeltaTime, 1.f));
		Plant3MatInstance->SetScalarParameterValue("Restoration", FMath::FInterpConstantTo(Plant3MatInstance->K2_GetScalarParameterValue("Restoration"), 0, DeltaTime, 1.f));
		Plant4MatInstance->SetScalarParameterValue("Restoration", FMath::FInterpConstantTo(Plant4MatInstance->K2_GetScalarParameterValue("Restoration"), 0, DeltaTime, 1.f));
	}
	else
	{
		RestoredParticle->Activate();
		AudioCreakComp->FadeOut(0.5f, 0);
		AudioShoreComp->FadeIn(0.5f, 1.f);
		bHopeRestoreComplete = true;
	}
}

void AHopesBloom_Actor::IncrementBlessingCheck()
{
	BlessingCheck++;
	if (BlessingCheck == BlessingAmountOriginal)
	{
		bHopeRestore = true;
		

		//Stops editor from crashing if no actor has been assigned
		if (WaterFieldRef != nullptr) 
		{
			GetWorldTimerManager().SetTimer(UnlockDoorTimerHandle, this, &AHopesBloom_Actor::UnlockDoor, 2.f, false, 0.f);
		}
		
	}
}

void AHopesBloom_Actor::UnlockDoor()
{
	WaterFieldRef->bChangeLockStatus = true;
	GetWorldTimerManager().ClearTimer(UnlockDoorTimerHandle);
}