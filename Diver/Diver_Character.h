// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Diver_AIController.h"
#include "Sound/SoundCue.h"
#include "Base211Character.h"
#include "Components/AudioComponent.h"
#include "Diver_Character.generated.h"

UCLASS()
class MINDATSEA_API ADiver_Character : public ACharacter
{
	GENERATED_BODY()


	//Last time the player was spotted 
	float LastSeenTime;

	//Last time the player was heard
	float LastHeardTime;

	//Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	//Resets after sense time-out to avoid unnecessary clearing of target each tick
	bool bSensedTarget;

	float TraceDistance;



public:
	// Sets default values for this character's properties
	ADiver_Character();

	UFUNCTION(BlueprintNativeEvent, Category = "AIVariables | Character")
	void GetPerceptionLocRot(FVector& OutLocation, FRotator& OutRotation) const;
	void GetPerceptionLocRot_Implementation(FVector& OutLocation, FRotator& OutRotation) const;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	//Update the vocal loop of the zombie (idle, wandering, hunting)
	UFUNCTION(Reliable, NetMulticast)
	void BroadcastUpdateAudioLoop(bool bNewSensedTarget);

	void BroadcastUpdateAudioLoop_Implementation(bool bNewSensedTarget);



	//Plays the idle, wandering or hunting sound
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioHuntLoopComp;

	//Distance sound
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioDistanceLoopComp;

	//Chase Theme
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioChaseThemeLoopComp;

	//Lost Theme
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioLostThemeComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetActorEyesViewPoint(FVector &Location, FRotator &Rotation) const override;

	// Plays the idle, wandering or hunting sound
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AudioLoopComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lights")
	UPointLightComponent* PointLightRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lights")
	UPointLightComponent* PointLightLeft;

	//Sets eye socket names
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName fnEyeSocketRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName fnEyeSocketLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lights")
	USpotLightComponent* SpotLightRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lights")
	USpotLightComponent* SpotLightLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	//Do once booleans
	bool doOnceTorchOn;

	bool doOnceTorchOff;

	bool doOnceCrouch;

	bool doOnceStand;

	bool doOncePlayHuntSound;

	float CurrentPerception;

private:

	UFUNCTION()
	void OnPlayerCaught(APawn* Pawn);

	

};
