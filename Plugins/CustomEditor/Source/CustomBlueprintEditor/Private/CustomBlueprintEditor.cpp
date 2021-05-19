// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomBlueprintEditor.h"

DEFINE_LOG_CATEGORY(CustomBlueprintEditor);

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

void FCustomBlueprintEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomBlueprintEditor, Warning, TEXT("CustomBlueprintEditor module has started!"));

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();

	EAssetTypeCategories::Type AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("MyBlueprint")), LOCTEXT("MyBlueprintCategory", "My Blueprint"));

	TSharedRef<IAssetTypeActions> AssetTypeAction = MakeShared<FAssetTypeActions_MyBlueprint>(AssetTypeCategory);

	RegisterAssetTypeAction(AssetTools, AssetTypeAction);
}

void FCustomBlueprintEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomBlueprintEditor, Warning, TEXT("CustomBlueprintEditor module has Shutdown!"));

	if (FAssetToolsModule::IsModuleLoaded())
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		for (TSharedRef<IAssetTypeActions> Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}

	RegisteredAssetTypeActions.Empty();
}

void FCustomBlueprintEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

	
IMPLEMENT_MODULE(FCustomBlueprintEditorModule, CustomBlueprintEditor)


#undef LOCTEXT_NAMESPACE