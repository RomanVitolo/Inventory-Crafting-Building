// Copyright Epic Games, Inc. All Rights Reserved.


#include "Unreal5ToolsPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Unreal5ToolsCameraManager.h"

AUnreal5ToolsPlayerController::AUnreal5ToolsPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AUnreal5ToolsCameraManager::StaticClass();
}

void AUnreal5ToolsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
