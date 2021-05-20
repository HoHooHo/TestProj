// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditorToolkit_MyBlueprint.h"
#include "EdGraphSchema_K2_MyBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ToolMenus.h"
#include "WorkflowOrientedApp/SModeWidget.h"
#include "CustomBlueprintStyle.h"


#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"


class SMyBlueprintModeSeparator : public SBorder
{
public:
	SLATE_BEGIN_ARGS(SMyBlueprintModeSeparator) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArg)
	{
		SBorder::Construct(
			SBorder::FArguments()
			.BorderImage(FCustomBlueprintStyle::GetBrush("CustomBlueprint.PipelineSeparator"))
			.Padding(0.0f)
		);
	}

	// SWidget interface
	virtual FVector2D ComputeDesiredSize(float) const override
	{
		const float Height = 20.0f;
		const float Thickness = 16.0f;
		return FVector2D(Thickness, Height);
	}
	// End of SWidget interface
};



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

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FAssetEditorToolkit_MyBlueprint::ExtendToolbarWidget)
	);

	AddToolbarExtender(ToolbarExtender);
}


void FAssetEditorToolkit_MyBlueprint::ExtendToolbarWidget(FToolBarBuilder& ignoreToolbarBuilder)
{
	/*
	FToolBarBuilder ToolbarBuilder(FCustomBlueprintAction::Get()->GetCommands(), FMultiBoxCustomization::None);
	ToolbarBuilder.AddToolBarButton(FCustomBlueprintCommands::Get().Mode1);
	ToolbarBuilder.AddSeparator();
	ToolbarBuilder.AddToolBarButton(FCustomBlueprintCommands::Get().Mode2);
	AddToolbarWidget(ToolbarBuilder.MakeWidget());
	*/

	TAttribute<FName> OnGetActiveMode(this, &FAssetEditorToolkit_MyBlueprint::GetCurrentMode);
	FOnModeChangeRequested OnSetActiveMode = FOnModeChangeRequested::CreateSP(this, &FAssetEditorToolkit_MyBlueprint::SetCurrentMode);

	// Left side padding
	AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));

	AddToolbarWidget(
		SNew(SModeWidget, FText::FromString(TEXT("Mode1")), FName("Mode111"))
		.OnGetActiveMode(OnGetActiveMode)
		.OnSetActiveMode(OnSetActiveMode)
		.ToolTipText(LOCTEXT("CustomBlueprintMode1Tooltip", "Switch to Mode1"))
		.IconImage(FCustomBlueprintStyle::GetBrush("CustomBlueprint.Mode1Icon"))
		.SmallIconImage(FCustomBlueprintStyle::GetBrush("CustomBlueprint.Mode1Icon.Small"))
		.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("Mode1")))
	);

	AddToolbarWidget(SNew(SMyBlueprintModeSeparator));

	AddToolbarWidget(
		SNew(SModeWidget, FText::FromString(TEXT("Mode2")), FName("Mode222"))
		.OnGetActiveMode(OnGetActiveMode)
		.OnSetActiveMode(OnSetActiveMode)
		.ToolTipText(LOCTEXT("CustomBlueprintMode2Tooltip", "Switch to Mode2"))
		.IconImage(FCustomBlueprintStyle::GetBrush("CustomBlueprint.Mode2Icon"))
		.SmallIconImage(FCustomBlueprintStyle::GetBrush("CustomBlueprint.Mode2Icon.Small"))
		.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("Mode2")))
	);

	// Right side padding
	AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));
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