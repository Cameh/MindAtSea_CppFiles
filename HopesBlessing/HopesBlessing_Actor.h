// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Base211Character.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "HopesBlessing_Actor.generated.h"

UCLASS()
class MINDATSEA_API AHopesBlessing_Actor : public AActor
{
	GENERATED_BODY()

	float ColourLerp;
	FLinearColor StartMatColour;
	FLinearColor FinishMatColour;
	FVector StartScale;
	FVector FinishScale;
	bool bColourChange;
	bool bCompletedCharge;
	bool bShakeOnce;
	bool bCheckShakeOnce;
	bool bScaleOnce;
	bool bScaledBig;

	FVector OriginalLoc;
	FVector OriginalScale;

	FTimerHandle ShakeTimerHandle;
	
public:	
	//Sets default values for this actor's properties
	AHopesBlessing_Actor();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Plays the hum sound
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLoopComp;

	//Plays the water bubble sound
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioCompleteComp;

public:	
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Plant1StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Plant2StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BallStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UParticleSystem* FinishParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UPointLightComponent* PointLight;

	void ChangeColourSwitch();

	void SmoothColourChange(FLinearColor FinishColour, float DeltaSeconds, bool bReverse, float InterpSpeed);

	void ChargedUp();

	void Shake();

	void ScaleChange(FVector Scale, float DeltaSeconds);

	ABase211Character* PlayerChar;
};
