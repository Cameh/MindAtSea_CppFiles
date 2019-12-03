// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "TargetPoint_Actor.generated.h"

UCLASS()
class MINDATSEA_API ATargetPoint_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetPoint_Actor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Zone)
	UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetPoint")
	int SequenceNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
