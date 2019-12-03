// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "SprintShake.h"

USprintShake::USprintShake()
{
	bSingleInstance = true;

	OscillationDuration = 0.1f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;

	RotOscillation.Pitch.Amplitude = 2.f;
	RotOscillation.Pitch.Frequency = 15.f;

	RotOscillation.Yaw.Amplitude = 0.f;
	RotOscillation.Yaw.Frequency = 5.f;

	RotOscillation.Roll.Amplitude = 1.f;
	RotOscillation.Roll.Frequency = 7.5f;

	LocOscillation.Z.Amplitude = 1.f;
	LocOscillation.Z.Frequency = 5.f;
}