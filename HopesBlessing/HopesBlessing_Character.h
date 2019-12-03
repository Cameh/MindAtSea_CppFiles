// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Base211Character.h"
#include "HopesBlessing_AIController.h"
#include "Components/PointLightComponent.h"
#include "HopesBlessing_Character.generated.h"

UCLASS()
class MINDATSEA_API AHopesBlessing_Character : public ACharacter
{
	GENERATED_BODY()

	float ColourLerp;
	float MoveToAILoc;
	FLinearColor StartMatColour;
	FLinearColor FinishMatColour;
	FVector StartScale;
	FVector FinishScale;
	FVector ShrinkScale;
	bool bColourChange;
	bool bCompletedCharge;

	//Do once booleans
	bool bShakeOnce;
	bool bCheckShakeOnce;
	bool bScaleOnce;
	bool bScaledBig;
	bool bIsShrink;
	bool bShrinkOnce;

	FVector OriginalLoc;
	FVector OriginalScale;
	

	FTimerHandle ShakeTimerHandle;

	FTimerDelegate ShrinkTimerDel;
	FTimerHandle ShrinkTimerHandle;

	


public:
	// Sets default values for this character's properties
	AHopesBlessing_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLoopComp;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioCompleteComp;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioHoverLoopComp;



	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystemComponent* OrbParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Plant1StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Plant2StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BallStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UParticleSystem* FinishParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UPointLightComponent* PointLight;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioBloomFoundComp;

	void ChangeColourSwitch();

	void SmoothColourChange(FLinearColor FinishColour, float DeltaSeconds, bool bReverse, float InterpSpeed);

	void ChargedUp();

	void Shake();

	void ScaleChange(FVector Scale, float DeltaSeconds);

	UFUNCTION()
	void ShrinkToAISetup();

	void ShrinkToAI(FVector Scale, float DeltaSeconds);
	

	ABase211Character* PlayerChar;

};
