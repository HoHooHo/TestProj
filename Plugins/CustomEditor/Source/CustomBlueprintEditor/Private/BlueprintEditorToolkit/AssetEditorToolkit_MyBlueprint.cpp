// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditorToolkit_MyBlueprint.h"
#include "EdGraphSchema_K2_MyBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ToolMenus.h"


#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"


void FAssetEditorToolkit_MyBlueprint::InitEditor_MyBlueprint(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	FBlueprintEditor::InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);

	//Extend Custom Menu
	ExtendMenu();
	//Extend Custom Toolbar
	ExtendToolbar();
	RegenerateMenusAndToolbars();
}


void FAssetEditorToolkit_MyBlueprint::ExtendMenu()
{
	if (MenuExtender.IsValid())
	{
		//RemoveMenuExtender(MenuExtender);
		//MenuExtender.Reset();
		return;
	}

	MenuExtender = MakeShareable(new FExtender());

	struct Local
	{
		static void AddMenu(FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.BeginSection("EditMyBlueprint", LOCTEXT("EditMyBlueprint", "Edit MyBlueprint"));
			MenuBuilder.AddMenuEntry(
				LOCTEXT("Test1", "Test 1"),
				LOCTEXT("Test1Tooltip", "Test1 Tooltip"),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateLambda([]() {
					FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test", "Test Test Test Test Test Test Test"));
					})
				),
				"Test1Host"
						);
			MenuBuilder.AddMenuSeparator();
			MenuBuilder.AddMenuEntry(
				LOCTEXT("Test2", "Test2 "),
				LOCTEXT("Test2Tooltip", "Test2 Tooltip"),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateLambda([]() {
					FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test2", "Test2222 Test Test 2222Test Test Test Test"));
					})
				)
			);
			MenuBuilder.EndSection();
		}

		static void AddMenuBar(FMenuBarBuilder& MenuBarBuilder)
		{
			MenuBarBuilder.AddPullDownMenu(
				LOCTEXT("MyBlueprint", "MyBlueprint"),
				LOCTEXT("MyBlueprintMenuTooltip", "MyBlueprint Menu Tooltip"),
				FNewMenuDelegate::CreateStatic(&Local::AddMenu),
				"MyBlueprint"
			);
		}
	};

	MenuExtender->AddMenuBarExtension(
		"Edit",
		EExtensionHook::After,
		GetToolkitCommands(),
		FMenuBarExtensionDelegate::CreateStatic(&Local::AddMenuBar)
	);
	AddMenuExtender(MenuExtender);
}

void FAssetEditorToolkit_MyBlueprint::ExtendToolbar()
{
	if (ToolbarExtender.IsValid())
	{
		return;
	}

	ToolbarExtender = MakeShareable(new FExtender());

	struct Local
	{
		static void AddToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("MyBPSection");
			ToolbarBuilder.AddToolBarButton(
				FUIAction(FExecuteAction::CreateLambda([]() {
					FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test", "Test Test Test Test Test Test Test"));
					})
				),
				"MyBtn",
				LOCTEXT("MyBPBarBtn", "MyBPBarBtn"),
				LOCTEXT("MyBPBarBtnTooltip", "MyBP Bar Btn Tooltip")
			);

			ToolbarBuilder.EndSection();
		}
	};

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateStatic(&Local::AddToolbar)
	);
	AddToolbarExtender(ToolbarExtender);
}



FName FAssetEditorToolkit_MyBlueprint::GetToolkitFName() const
{
	return FName(TEXT("FAssetEditorToolkit_MyBlueprint"));
}

FText FAssetEditorToolkit_MyBlueprint::GetBaseToolkitName() const
{
	return LOCTEXT("FAssetEditorToolkit_MyBlueprint", "MyAsset Editor");
}

FString FAssetEditorToolkit_MyBlueprint::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("FAssetEditorToolkit_MyBlueprint_MyAssetWorldCentricTabPrefix", "MyBlueprint-").ToString();
}

#undef LOCTEXT_NAMESPACE