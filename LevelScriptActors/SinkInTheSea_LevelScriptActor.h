// Made by Cameron Harford. All sound rights go to their respectful creators.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "SinkInTheSea_LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class MINDATSEA_API ASinkInTheSea_LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "LevelBlueprint")
		void ForcePlayerView();

};
