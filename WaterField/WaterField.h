// Fill out your copyrig// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Base211Character.h"
#include "Components/BillboardComponent.h"
#include "WaterField.generated.h"

UCLASS()
class MINDATSEA_API AWaterField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterField();

	void UnlockDoor(float DeltaTime);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Zone)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Zone)
	UBoxComponent* RevertBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Zone)
	UStaticMeshComponent* PlaneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Zone)
	UBillboardComponent* Billboard;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapRevert(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Zone)
	bool UseRevertFirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Zone)
	bool bIsLocked;

	bool bChangeLockStatus;

	UMaterialInstanceDynamic* Plane1MatInstance;

};
