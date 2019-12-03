// Made by Cameron Harford. All sound rights go to their respectful creators.


#include "SinkInTheSea_LevelScriptActor.h"
#include "Base211Character.h"

void ASinkInTheSea_LevelScriptActor::ForcePlayerView()
{
	GetWorld()->GetFirstPlayerController()->SetControlRotation(FRotator(90.f, 90.f, 0.f));
}