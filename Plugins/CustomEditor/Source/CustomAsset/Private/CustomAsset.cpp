// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAsset.h"

DEFINE_LOG_CATEGORY(CustomAsset);

#define LOCTEXT_NAMESPACE "CustomAsset"

void FCustomAssetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomAsset, Warning, TEXT("CustomAsset module has started!"));
}

void FCustomAssetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomAsset, Warning, TEXT("CustomAsset module has Shutdown!"));
}

	
IMPLEMENT_MODULE(FCustomAssetModule, CustomAsset)


#undef LOCTEXT_NAMESPACE