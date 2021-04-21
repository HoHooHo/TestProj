// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAsset.h"
#include "AssetTypeActions_MyAsset.h"

DEFINE_LOG_CATEGORY(CustomAsset);

#define LOCTEXT_NAMESPACE "CustomAsset"

void FCustomAssetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomAsset, Warning, TEXT("CustomAsset module has started!"));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("MyCustom")), LOCTEXT("MyCustomAssetCategory", "My Custom"));

	TSharedRef<IAssetTypeActions> AssetTypeAction = MakeShareable(new FAssetTypeActions_MyAsset(AssetTypeCategory));

	RegisterAssetTypeAction(AssetTools, AssetTypeAction);
}

void FCustomAssetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomAsset, Warning, TEXT("CustomAsset module has Shutdown!"));

	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (TSharedPtr<IAssetTypeActions> Action : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
}



void FCustomAssetModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}



	
IMPLEMENT_MODULE(FCustomAssetModule, CustomAsset)


#undef LOCTEXT_NAMESPACE