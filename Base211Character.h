// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "Base211Character.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ABase211Character : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	ABase211Character();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void SlowWalk();

	void RevertWalk();

	void HopesBlessingShakeEvent();

	void Died();

protected:
	virtual void BeginPlay();

public:

	//Camera Category
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FRotator CameraRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bLookingAtActor;

	//Torch Category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Torch)
	class USpotLightComponent* TorchLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Torch)
	class USpotLightComponent* TorchLightFar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Torch)
	class UPointLightComponent* LightVicinity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Torch)
	bool TorchToggle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Torch)
	bool IntensityIncrease;

	//Movement Category
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsSlowWalking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	bool IsEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool TooFast;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	bool bCanPause;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	float MasterVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float TargetBEH;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float TargetCapsuleSize;

	//Shakes
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> WalkShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> SlowWalkShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CrouchWalkShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> SlowCrouchWalkShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> SprintShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> SlowSprintShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> FallShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> HopesBlessingShake;

	//BP Component
	UPROPERTY(BlueprintReadOnly, Category = Setup)
	UActorComponent* AquaticBoyuancy = nullptr;

	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* StepAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* StepAudioCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<APawn*> HopesBlessingArray;

	//Pause Menu
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> PauseWidgetBPReference;


protected:

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void OnSprint();

	void StopSprint();

	void LightIntensify(float TargetHeight, float DeltaSeconds, float InnerTarget, float OuterTarget, float IntensityTarget, float FOV, float Vignette);

	void TorchDim(float VicinityTarget, float DeltaSeconds, float FarTarget, float NearTarget, float FOV, float Vignette);

	void FocusLight();

	void ToggleLight();

	void ReturnFocusLight();

	void ToggleCrouch();

	virtual void Landed(const FHitResult& Hit) override;

	void SwimUp(float Scale);

	void PauseMenu();

	
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* LandAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundCue* LandAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* WaterBreathAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundCue* WaterBreathAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* WaterRegionEnterAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundCue* WaterRegionEnterAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundCue* WaterRegionExitAudioCue;

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

