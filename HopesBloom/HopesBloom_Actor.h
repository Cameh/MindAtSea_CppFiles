// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base211Character.h"
#include "Components/PointLightComponent.h"
#include "WaterField/WaterField.h"
#include "Components/BoxComponent.h"
#include "HopesBloom_Actor.generated.h"

UCLASS()
class MINDATSEA_API AHopesBloom_Actor : public AActor
{
	GENERATED_BODY()

	
	bool bHopeRestoreComplete;

	int BlessingCheck;
	int BlessingAmountOriginal;

	FTimerHandle UnlockDoorTimerHandle;

public:	
	// Sets default values for this actor's properties
	AHopesBloom_Actor();


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Restore Hope Function")
	void RestoreHope(float DeltaTime);

	void IncrementBlessingCheck();

	void UnlockDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioCreakComp;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioShoreComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lights")
	UPointLightComponent* PointLightBlue;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		USceneComponent* SceneGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Plant1StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Plant2StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Plant3StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Plant4StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UBoxComponent* AcceptanceBox;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* RestoredParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* GodRayParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blessings")
		int BlessingAmount;

	//Allow for water field unlock and colour change
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blessings")
	bool bHopeRestore;

	//Material instances
	UMaterialInstanceDynamic* Plant1MatInstance;
	UMaterialInstanceDynamic* Plant2MatInstance;
	UMaterialInstanceDynamic* Plant3MatInstance;
	UMaterialInstanceDynamic* Plant4MatInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	AWaterField* WaterFieldRef;
};
