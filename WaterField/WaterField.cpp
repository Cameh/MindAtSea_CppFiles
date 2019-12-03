// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "WaterField.h"

// Sets default values
AWaterField::AWaterField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Billboard = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RevertBox = CreateDefaultSubobject<UBoxComponent>(FName("RevertBox"));
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlaneMesh"));
	SetRootComponent(Billboard);
	RevertBox->SetupAttachment(Billboard);
	PlaneMesh->SetupAttachment(Billboard);	
}

// Called when the game starts or when spawned
void AWaterField::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWaterField::OnOverlap);
	RevertBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterField::OnOverlapRevert);

	UMaterialInterface* Plane1Material = PlaneMesh->GetMaterial(0);

	Plane1MatInstance = PlaneMesh->CreateDynamicMaterialInstance(0, Plane1Material);
	if (!bIsLocked)
	{
		if (UseRevertFirst == true)
		{
			//Set exit water field first
			Box->SetGenerateOverlapEvents(false);
			RevertBox->SetGenerateOverlapEvents(true);
		}
		else
		{
			//Set enter water field first
			Box->SetGenerateOverlapEvents(true);
			RevertBox->SetGenerateOverlapEvents(false);
		}
	}
	else
	{
		//Set all to false as it is locked
		Box->SetGenerateOverlapEvents(false);
		RevertBox->SetGenerateOverlapEvents(false);
	}
	
	if (bIsLocked)
	{
		//Locked so set block all collision and set colour to red
		PlaneMesh->SetCollisionResponseToAllChannels(ECR_Block);
		Plane1MatInstance->SetScalarParameterValue("RefractionBlend", 1.f);
	}
	else
	{
		//Unlocked so set all collision to ignore (Default colour is blue so no need for change)
		PlaneMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PlaneMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}

	
}

// Called every frame
void AWaterField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bChangeLockStatus)
	{
		UnlockDoor(DeltaTime);
	}

}

void AWaterField::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	UWorld* WorldRef = GetWorld();
	ABase211Character* CharacterRef = Cast<ABase211Character>(WorldRef->GetFirstPlayerController()->GetCharacter());
	if (CharacterRef)
	{
		if (OtherActor == CharacterRef)
		{
			//Set enter water. Disable this collision box and enable the exit water box
			CharacterRef->SlowWalk();
			Box->SetGenerateOverlapEvents(false);
			RevertBox->SetGenerateOverlapEvents(true);
		}
	}
	
}

void AWaterField::OnOverlapRevert(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* WorldRef = GetWorld();
	ABase211Character* CharacterRef = Cast<ABase211Character>(WorldRef->GetFirstPlayerController()->GetCharacter());
	if (CharacterRef)
	{
		if (OtherActor == CharacterRef)
		{
			//Set exit water. Disable this collision box and enable the enter water box
			CharacterRef->RevertWalk();
			Box->SetGenerateOverlapEvents(true);
			RevertBox->SetGenerateOverlapEvents(false);
		}
	}

}

void AWaterField::UnlockDoor(float DeltaTime)
{
	
	if (Plane1MatInstance->K2_GetScalarParameterValue("RefractionBlend") != 0)
	{
		//Smoothly change colour from red to blue
		Plane1MatInstance->SetScalarParameterValue("RefractionBlend", FMath::FInterpConstantTo(Plane1MatInstance->K2_GetScalarParameterValue("RefractionBlend"), 0, DeltaTime, 1.f));
	}
	else
	{
		//Unlock once colour change is complete
		PlaneMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		PlaneMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		bChangeLockStatus = false;
		bIsLocked = false;

		if (UseRevertFirst == true)
		{
			Box->SetGenerateOverlapEvents(false);
			RevertBox->SetGenerateOverlapEvents(true);
		}
		else
		{
			Box->SetGenerateOverlapEvents(true);
			RevertBox->SetGenerateOverlapEvents(false);
		}
	}
}