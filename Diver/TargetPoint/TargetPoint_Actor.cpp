// Made by Cameron Harford. All sound rights go to their respectful creators.

#include "TargetPoint_Actor.h"

// Sets default values
ATargetPoint_Actor::ATargetPoint_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
	SetRootComponent(Billboard);
	SequenceNumber = 0;
}

// Called when the game starts or when spawned
void ATargetPoint_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetPoint_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

