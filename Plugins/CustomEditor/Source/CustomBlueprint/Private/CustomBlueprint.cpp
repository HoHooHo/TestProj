// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomBlueprint.h"

DEFINE_LOG_CATEGORY(CustomBlueprint);

#define LOCTEXT_NAMESPACE "CustomBlueprint"

void FCustomBlueprintModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomBlueprint, Warning, TEXT("CustomBlueprint module has started!"));
}

void FCustomBlueprintModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomBlueprint, Warning, TEXT("CustomBlueprint module has Shutdown!"));
}

	
IMPLEMENT_MODULE(FCustomBlueprintModule, CustomBlueprint)


#undef LOCTEXT_NAMESPACE