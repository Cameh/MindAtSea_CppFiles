// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Base211Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "HopesBlessing/HopesBlessing_Character.h"
#include "GameFramework/InputSettings.h"
#include "Diver/Diver_Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ABase211Character

ABase211Character::ABase211Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Set line trace distance
	TraceDistance = 500.f;

	//Set Master Volume
	MasterVolume = 1.f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(FirstPersonCameraComponent);
	CameraBoom->bAbsoluteRotation = false; // Want arm to rotate when character does
	CameraBoom->TargetArmLength = -75.0f;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 20.f;
	CameraBoom->CameraRotationLagSpeed = 25.f;

	//Create torch
	TorchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLight"));
	TorchLight->SetupAttachment(CameraBoom);
	TorchLight->bAbsoluteRotation = false;

	//Create torch 2
	TorchLightFar = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLightFar"));
	TorchLightFar->SetupAttachment(CameraBoom);
	TorchLightFar->bAbsoluteRotation = false;

	//Create vicinity light
	LightVicinity = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightVicinity"));
	LightVicinity->SetupAttachment(GetCapsuleComponent());
	LightVicinity->bAbsoluteRotation = false;

	
	TargetBEH = IsCrouching ? CrouchedEyeHeight : 96;
	TargetCapsuleSize = IsCrouching ? GetCharacterMovement()->CrouchedHalfHeight : 40;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	AquaticBoyuancy = CreateDefaultSubobject<UActorComponent>(TEXT("AquaticBoyuancy"));

	//Sound Setup

	//Land Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> FallLandCue(TEXT("'/Game/Sounds/FallLand_Cue'"));
	LandAudioCue = FallLandCue.Object;
	LandAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LandAudioComp"));
	// I don't want the sound playing the moment it's created.
	LandAudioComponent->bAutoActivate = false;
	// I want the sound to follow the pawn around, so I attach it to the Pawns root.
	LandAudioComponent->SetupAttachment(GetCapsuleComponent());
	// I want the sound to come from slighty in front of the pawn.
	LandAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//Water Breath Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> WaterBreathCue(TEXT("'/Game/Sounds/WaterBreathing_Cue'"));
	WaterBreathAudioCue = WaterBreathCue.Object;
	WaterBreathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WaterBreathAudioComp"));
	WaterBreathAudioComponent->bAutoActivate = false;
	WaterBreathAudioComponent->SetupAttachment(GetCapsuleComponent());
	WaterBreathAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//Water Region Enter Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> WaterRegionEnterCue(TEXT("'/Game/Sounds/WaterRegionEnter_Cue'"));
	WaterRegionEnterAudioCue = WaterRegionEnterCue.Object;
	WaterRegionEnterAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WaterRegionEnterAudioComp"));
	WaterRegionEnterAudioComponent->bAutoActivate = false;
	WaterRegionEnterAudioComponent->SetupAttachment(GetCapsuleComponent());
	WaterRegionEnterAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	//Water Region Enter Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> WaterRegionExitCue(TEXT("'/Game/Sounds/WaterRegionExit_Cue'"));
	WaterRegionExitAudioCue = WaterRegionExitCue.Object;

	//Step Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> StepCue(TEXT("'/Game/Sounds/Steps/Mud/MudStep_Cue'"));
	StepAudioCue = StepCue.Object;
	StepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepAudioComp"));
	StepAudioComponent->bAutoActivate = false;
	StepAudioComponent->SetupAttachment(GetCapsuleComponent());
	StepAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	
}

void ABase211Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (LandAudioCue->IsValidLowLevelFast()) {
		LandAudioComponent->SetSound(LandAudioCue);
	}

	if (WaterBreathAudioCue->IsValidLowLevelFast()) {
		WaterBreathAudioComponent->SetSound(WaterBreathAudioCue);
	}

	if (StepAudioCue->IsValidLowLevelFast()) {
		StepAudioComponent->SetSound(StepAudioCue);
	}

	if (WaterRegionEnterAudioCue->IsValidLowLevelFast()) {
		WaterRegionEnterAudioComponent->SetSound(WaterRegionEnterAudioCue);
	}
}

void ABase211Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

	TorchToggle = true;

}

//Tick Event
void ABase211Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Torch Intensity Increase
	if (IntensityIncrease == true && TorchLightFar->Intensity != 3000.f)
	{
		LightIntensify(3000.f, DeltaTime, 5.f, 10.f, 300.f, 85.f, 0.8f);
	}

	//If right click is held (Regardless of intensity time) make a line trace
	else if (IntensityIncrease)
	{
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,        //result
			FirstPersonCameraComponent->GetComponentLocation(),    //start
			FirstPersonCameraComponent->GetForwardVector() * TraceDistance + FirstPersonCameraComponent->GetComponentLocation(), //end
			ECC_WorldStatic, //collision channel
			RV_TraceParams
		);

		RV_Hit.bBlockingHit; //did hit something? (bool)
		RV_Hit.GetActor(); //the hit actor if there is one
		RV_Hit.ImpactPoint;  //FVector
		RV_Hit.ImpactNormal;  //FVector
	
		if (RV_Hit.GetActor())
		{
			AHopesBlessing_Character* HopesBlessingActor = Cast<AHopesBlessing_Character>(RV_Hit.GetActor());

			//Hit the Hopes Blessing Actor
			if (RV_Hit.bBlockingHit && RV_Hit.GetActor() == HopesBlessingActor)
			{
				
				bLookingAtActor = true;
				HopesBlessingActor->ChangeColourSwitch();

			}

			//Looking at another object
			else
			{
				bLookingAtActor = false;
			}
		}

		else
		{
			bLookingAtActor = false;
		}
	}

	else if (IntensityIncrease == false && TorchLightFar->Intensity != 0.f)
	{
		if (TorchToggle == false)
		{
			LightIntensify(0.f, DeltaTime, 15.f, 45.f, 1000.f, 90.f, 0.4f);
			bLookingAtActor = false;
		}
		else
		{
			LightIntensify(0.f, DeltaTime, 15.f, 45.f, 0.f, 90.f, 0.4f);
			bLookingAtActor = false;
		}
	}

	//Crouch Toggle
	if (IsCrouching)
	{
		
		if (Controller != NULL)
		{
			BaseEyeHeight = FMath::FInterpTo(BaseEyeHeight, TargetBEH, DeltaTime, 10.0f);
			GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::FInterpTo(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleSize, DeltaTime, 10.0f), true);
			// Dist and DeltaMovCaps are used for the interpolation value added to RelativeLocation.Z
			const float Dist = TargetCapsuleSize - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			const float DeltaMovCaps = Dist * FMath::Clamp<float>(DeltaTime * 10.0f, 0.f, 1.f);
			GetCapsuleComponent()->SetRelativeLocation(FVector(GetCapsuleComponent()->RelativeLocation.X, GetCapsuleComponent()->RelativeLocation.Y, (GetCapsuleComponent()->RelativeLocation.Z + DeltaMovCaps)), true);
		}
	}

	else if (!IsCrouching)
	{
		
		if (Controller != NULL)
		{
			BaseEyeHeight = FMath::FInterpTo(BaseEyeHeight, TargetCapsuleSize, DeltaTime, 10.0f);
			GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::FInterpTo(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetBEH, DeltaTime, 10.0f), true);
			// Dist and DeltaMovCaps are used for the interpolation value added to RelativeLocation.Z
			const float Dist = TargetCapsuleSize - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			const float DeltaMovCaps = Dist * FMath::Clamp<float>(DeltaTime * 10.0f, 0.f, 1.f);
			GetCapsuleComponent()->SetRelativeLocation(FVector(GetCapsuleComponent()->RelativeLocation.X, GetCapsuleComponent()->RelativeLocation.Y, (GetCapsuleComponent()->RelativeLocation.Z + DeltaMovCaps)), true);
		}
	}

	//Torch Toggle
	if (TorchToggle == true && LightVicinity->Intensity != 1000.f)
	{
		TorchDim(1000.f, DeltaTime, 0.f, 0.f, 90.f, 0.4f);
	}

	else if (TorchToggle == false && LightVicinity->Intensity != 0.f)
	{
		TorchDim(0.f, DeltaTime, 0.f, 1000.f, 90.f, 0.4f);
	}

	if (GetCharacterMovement()->IsFalling() == true)
	{
		if (GetCharacterMovement()->Velocity.Z < -200.f)
		{
			//Enable drop sound
			TooFast = true;

		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABase211Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ABase211Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABase211Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABase211Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABase211Character::LookUpAtRate);

	PlayerInputComponent->BindAxis("SwimUp", this, &ABase211Character::SwimUp);

	//Bind Sprint event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABase211Character::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABase211Character::StopSprint);

	//Focus Light Event
	PlayerInputComponent->BindAction("FocusLight", IE_Pressed, this, &ABase211Character::FocusLight);
	PlayerInputComponent->BindAction("FocusLight", IE_Released, this, &ABase211Character::ReturnFocusLight);

	//Toggle Light event
	PlayerInputComponent->BindAction("Torch", IE_Pressed, this, &ABase211Character::ToggleLight);

	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABase211Character::ToggleCrouch);

	//Pause
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ABase211Character::PauseMenu);
}

void ABase211Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		if (IsSlowWalking && GetCharacterMovement()->IsSwimming())
		{
			AddMovementInput(FVector(FirstPersonCameraComponent->GetForwardVector().X, FirstPersonCameraComponent->GetForwardVector().Y, FirstPersonCameraComponent->GetForwardVector().Z), Value / 4);
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), Value);
			
		}

		
		//Underwater Speed Settings
		if (WalkShake != NULL && !IsCrouching && !IsSprinting)
		{
			if (IsSlowWalking)
			{
				GetCharacterMovement()->MaxWalkSpeed = 100.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(SlowWalkShake, 1.0f);
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = 120.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WalkShake, 1.0f);
			}
		}

		else if (CrouchWalkShake != NULL && IsCrouching && !IsSprinting)
		{
			if (IsSlowWalking) 
			{

				GetCharacterMovement()->MaxWalkSpeed = 50.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(SlowCrouchWalkShake, 1.0f);

			}
			
			else 
			{

				GetCharacterMovement()->MaxWalkSpeed = 100.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CrouchWalkShake, 1.0f);

			}
		}

		else if (SprintShake != NULL && !IsCrouching && IsSprinting)
		{
			if (IsSlowWalking)
			{
				GetCharacterMovement()->MaxWalkSpeed = 150.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(SlowSprintShake, 1.0f);
			}

			else
			{
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(SprintShake, 1.0f);
			}
		}
	}
}

void ABase211Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABase211Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABase211Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABase211Character::SwimUp(float Scale)
{
	
}

//Sprint
void ABase211Character::OnSprint()
{
	
	if (!IsCrouching)
	{
		IsSprinting = true;
	}

	
}

void ABase211Character::StopSprint()
{
	if(!IsCrouching)
	IsSprinting = false;
}

void ABase211Character::LightIntensify(float TargetHeight, float DeltaSeconds, float InnerTarget, float OuterTarget, float IntensityTarget, float FOV, float Vignette)
{
	//Adjust light intensity for focus torch
	TorchLightFar->SetIntensity(FMath::FInterpConstantTo(TorchLightFar->Intensity, TargetHeight, DeltaSeconds, 10500.f));

	TorchLight->InnerConeAngle = (FMath::FInterpConstantTo(TorchLight->InnerConeAngle, InnerTarget, DeltaSeconds, 4500.f));

	TorchLight->OuterConeAngle = (FMath::FInterpConstantTo(TorchLight->OuterConeAngle, OuterTarget, DeltaSeconds, 4500.f));

	TorchLight->SetIntensity(FMath::FInterpConstantTo(TorchLight->Intensity, IntensityTarget, DeltaSeconds, 4500.f));

	FirstPersonCameraComponent->SetFieldOfView(FMath::FInterpConstantTo(FirstPersonCameraComponent->FieldOfView, FOV, DeltaSeconds, 10.f));

	FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = FMath::FInterpConstantTo(FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity, Vignette, DeltaSeconds, 1.f);

}

void ABase211Character::TorchDim(float VicinityTarget, float DeltaSeconds, float FarTarget, float NearTarget, float FOV, float Vignette)
{
	//Adjust light intensity for default torch
	TorchLightFar->SetIntensity(FMath::FInterpConstantTo(TorchLightFar->Intensity, FarTarget, DeltaSeconds, 10500.f));

	TorchLight->SetIntensity(FMath::FInterpConstantTo(TorchLight->Intensity, NearTarget, DeltaSeconds, 10500.f));

	LightVicinity->SetIntensity(FMath::FInterpConstantTo(LightVicinity->Intensity, VicinityTarget, DeltaSeconds, 10500.f));

	FirstPersonCameraComponent->SetFieldOfView(FMath::FInterpConstantTo(FirstPersonCameraComponent->FieldOfView, FOV, DeltaSeconds, 10.f));

	FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = FMath::FInterpConstantTo(FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity, Vignette, DeltaSeconds, 1.f);
}

//Enable focus light
void ABase211Character::FocusLight()
{
	if (TorchToggle == false) 
	{
		IntensityIncrease = true;
	}
}

void ABase211Character::ReturnFocusLight()
{
	if (TorchToggle == false) 
	{
		IntensityIncrease = false;
	}
}

//Enable toggle light
void ABase211Character::ToggleLight()
{
	if (!TorchToggle)
	{
		TorchToggle = true;
		IntensityIncrease = false;
	}
	else
	{
		TorchToggle = false;
	}

}

//Crouching
void ABase211Character::ToggleCrouch()
{
	if (IsCrouching)
	{
		//UnCrouch();
		IsCrouching = false;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		
	}
	else
	{
		if (IsSprinting == false) 
		{
			//Crouch();
			IsCrouching = true;
		}
		
	}
}

void ABase211Character::SlowWalk()
{
	//Enter water field sound and speed setup
	IsSlowWalking = true;
	WaterBreathAudioComponent->Play();
	if (WaterRegionEnterAudioCue->IsValidLowLevelFast()) 
	{
		WaterRegionEnterAudioComponent->SetSound(WaterRegionEnterAudioCue);
	}
	WaterRegionEnterAudioComponent->Play();
}

void ABase211Character::RevertWalk()
{
	IsSlowWalking = false;
	WaterBreathAudioComponent->FadeOut(2.f, 0.f);
	if (WaterRegionEnterAudioCue->IsValidLowLevelFast())
	{
		WaterRegionEnterAudioComponent->SetSound(WaterRegionExitAudioCue);
	}
	WaterRegionEnterAudioComponent->Play();
}

void ABase211Character::Landed(const FHitResult& Hit)
{
	// make sure the parent class still does its things
	Super::Landed(Hit);

	if (TooFast == true)
	{
		if (bCanPause)
		{
			//Play landing sound
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(FallShake, 1.0f);
			LandAudioComponent->Play();
			
		}
		TooFast = false;
	}
}

void ABase211Character::HopesBlessingShakeEvent()
{
	//Play when the Hopes Blessing is charged
	ReturnFocusLight();
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(HopesBlessingShake, 1.0f);
}

void ABase211Character::Died()
{
	IsDead = true;
}

//Create Pause Widget
void ABase211Character::PauseMenu()
{
	if (bCanPause)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseWidgetBPReference);
		WidgetInstance->AddToViewport();
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}