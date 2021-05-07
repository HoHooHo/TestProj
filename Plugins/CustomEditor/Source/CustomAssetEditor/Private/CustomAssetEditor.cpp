// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetEditor.h"
#include "AssetTypeActions_MyAsset.h"
#include "Style_MyAsset.h"
#include "PropertyEditorModule.h" 
#include "DetailCustomization_MyAsset.h"
#include "MyAsset.h"

DEFINE_LOG_CATEGORY(CustomAssetEditor);

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

void FCustomAssetEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(CustomAssetEditor, Warning, TEXT("CustomAssetEditor module has started!"));

	FStyle_MyAsset::Initialize();
	FStyle_MyAsset::ReloadTextures();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("MyCustom")), LOCTEXT("MyCustomAssetEditorCategory", "My Custom"));

	TSharedRef<IAssetTypeActions> AssetTypeAction = MakeShareable(new FAssetTypeActions_MyAsset(AssetTypeCategory));

	RegisterAssetTypeAction(AssetTools, AssetTypeAction);


	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(UMyAsset::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FDetailCustomization_MyAsset::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FCustomAssetEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(CustomAssetEditor, Warning, TEXT("CustomAssetEditor module has Shutdown!"));

	// Make sure we unregister the class layout 
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.UnregisterCustomClassLayout(UMyAsset::StaticClass()->GetFName());
	}


	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (TSharedPtr<IAssetTypeActions> Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}

	RegisteredAssetTypeActions.Empty();

	FStyle_MyAsset::Shutdown();
}



void FCustomAssetEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}



	
IMPLEMENT_MODULE(FCustomAssetEditorModule, CustomAssetEditor)


#undef LOCTEXT_NAMESPACE