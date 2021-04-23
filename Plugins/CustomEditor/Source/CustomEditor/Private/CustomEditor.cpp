// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditor.h"
#include "CustomEditorStyle.h"
#include "CustomEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SCustomCompundWidget.h"

static const FName CustomEditorTabName("CustomEditor");

#define LOCTEXT_NAMESPACE "FCustomEditorModule"

void FCustomEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCustomEditorStyle::Initialize();
	FCustomEditorStyle::ReloadTextures();

	FCustomEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCustomEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCustomEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CustomEditorTabName, FOnSpawnTab::CreateRaw(this, &FCustomEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCustomEditorTabTitle", "CustomEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FCustomEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCustomEditorStyle::Shutdown();

	FCustomEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CustomEditorTabName);
}

TSharedRef<SDockTab> FCustomEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCustomEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("CustomEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			/*SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CreateMyAsset", "Create My Asset"))
				]
			]*/

			SNew(SCustomCompundWidget)
		];
}

void FCustomEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CustomEditorTabName);
}

void FCustomEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCustomEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomEditorModule, CustomEditor)