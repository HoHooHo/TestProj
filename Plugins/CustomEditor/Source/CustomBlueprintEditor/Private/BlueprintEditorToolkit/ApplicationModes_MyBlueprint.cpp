// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationModes_MyBlueprint.h"
#include "AssetEditorToolkit_MyBlueprint.h"
#include "BlueprintEditor.h"
#include "BlueprintEditorTabs.h"
#include "BlueprintEditorSharedTabFactories.h"
#include "SBlueprintEditorToolbar.h"
#include "MyBlueprintEditorTabFactories.h"
#include "BlueprintEditorSharedTabFactories.h"



#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

const FName FApplicationModes_MyBlueprint::ModeID1(TEXT("ModeID1"));
const FName FApplicationModes_MyBlueprint::ModeID2(TEXT("ModeID2"));


FApplicationMode_MyBlueprint_Mode1::FApplicationMode_MyBlueprint_Mode1(TSharedPtr<FAssetEditorToolkit_MyBlueprint> InAssetEditorToolkit, FName InModeName, FText(*GetLocalizedMode)(const FName), const bool bRegisterViewport /* = true */, const bool bRegisterDefaultsTab /* = true */)
	: FApplicationMode(InModeName, GetLocalizedMode)
{
	AssetEditorToolkit = InAssetEditorToolkit;

	// Create the tab factories

	// Create the tab factories
	//EditorTabFactories.RegisterFactory(MakeShareable(new FDetailsTabSummoner_MyBlueprint(InAssetEditorToolkit)));
	EditorTabFactories.RegisterFactory(MakeShareable(new FCompilerResultsSummoner(InAssetEditorToolkit)));

	TabLayout = FTabManager::NewLayout("ApplicationMode_MyBlueprint_Mode1_Layout_V5")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.15f)
					->AddTab(FDetailsTabSummoner_MyBlueprint::ID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.6f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.2f)
						->SetHideTabWell(true)
						->AddTab(InAssetEditorToolkit->GetToolbarTabId(), ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.50f)
						->AddTab(FDetailsTabSummoner_MyBlueprint::ID, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.30f)
						->AddTab(FBlueprintEditorTabs::CompilerResultsID, ETabState::ClosedTab)
					)

				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.15f)
					->AddTab(FDetailsTabSummoner_MyBlueprint::ID, ETabState::OpenedTab)
					->AddTab(FDetailsTabSummoner_MyBlueprint::ID, ETabState::ClosedTab)
				)
			)
		);
}


void FApplicationMode_MyBlueprint_Mode1::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	FApplicationMode::RegisterTabFactories(InTabManager);

	TSharedPtr<FAssetEditorToolkit_MyBlueprint> Editor = AssetEditorToolkit.Pin();

	Editor->RegisterToolbarTab(InTabManager.ToSharedRef());
	Editor->PushTabFactories(EditorTabFactories);
}


void FApplicationMode_MyBlueprint_Mode1::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();


}

void FApplicationMode_MyBlueprint_Mode1::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}


FApplicationMode_MyBlueprint_Mode2::FApplicationMode_MyBlueprint_Mode2(TSharedPtr<FAssetEditorToolkit_MyBlueprint> InAssetEditorToolkit, FName InModeName, FText(*GetLocalizedMode)(const FName), const bool bRegisterViewport /* = true */, const bool bRegisterDefaultsTab /* = true */)
	: FBlueprintEditorApplicationMode(InAssetEditorToolkit, InModeName, GetLocalizedMode, bRegisterViewport, bRegisterDefaultsTab)
{
}

#undef LOCTEXT_NAMESPACE