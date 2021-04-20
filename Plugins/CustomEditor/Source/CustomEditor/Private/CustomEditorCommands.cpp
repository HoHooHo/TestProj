// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorCommands.h"

#define LOCTEXT_NAMESPACE "FCustomEditorModule"

void FCustomEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CustomEditor", "Bring up CustomEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
