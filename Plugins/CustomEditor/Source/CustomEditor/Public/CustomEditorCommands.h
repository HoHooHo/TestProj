// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CustomEditorStyle.h"

class FCustomEditorCommands : public TCommands<FCustomEditorCommands>
{
public:

	FCustomEditorCommands()
		: TCommands<FCustomEditorCommands>(TEXT("CustomEditor"), NSLOCTEXT("Contexts", "CustomEditor", "CustomEditor Plugin"), NAME_None, FCustomEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};