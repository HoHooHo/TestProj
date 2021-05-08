// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorButtonsCommands.h"

#define LOCTEXT_NAMESPACE "CustomEditorButtons"

void FCustomEditorButtonsCommands::RegisterCommands()
{
	UI_COMMAND(Button1, "DiaplayButton1", "My Button 1", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(Button2, "DiaplayButton2", "My Button 2", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(Button3, "DiaplayButton3", "My Button 3", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(Button4, "DiaplayButton4", "My Button 4", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
