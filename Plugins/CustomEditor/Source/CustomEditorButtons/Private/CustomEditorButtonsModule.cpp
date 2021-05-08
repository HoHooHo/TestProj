// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorButtonsModule.h"
#include "CustomEditorButtonsAction.h"

DEFINE_LOG_CATEGORY(CustomEditorButtons)

static const FName CustomEditorTabName("CustomEditorButtons");

#define LOCTEXT_NAMESPACE "CustomEditorButtons"

void FCustomEditorButtonsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomEditorButtons, Warning, TEXT("CustomAssetEditor module has started!"));

	CustomEditorButtonsAction = MakeShareable(new FCustomEditorButtonsAction());
	CustomEditorButtonsAction->Startup();
}

void FCustomEditorButtonsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomEditorButtons, Warning, TEXT("CustomAssetEditor module has Shutdown!"));

	if (CustomEditorButtonsAction.IsValid())
	{
		CustomEditorButtonsAction->Shutdown();
	}
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_GAME_MODULE(FCustomEditorButtonsModule, CustomEditorButtons)