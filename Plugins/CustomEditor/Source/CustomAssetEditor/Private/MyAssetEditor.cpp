// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyAssetEditor.h"
#include "MyAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "SMyAssetViewport.h"


#define LOCTEXT_NAMESPACE "CustomAssetEditor"

const FName FMyAssetEditor::DetailsTabId = TEXT("MyAssetEditor.DetailsTabId");
const FName FMyAssetEditor::GraphTabId = TEXT("MyAssetEditor.GraphTabId");
const FName FMyAssetEditor::ViewportId = TEXT("MyAssetEditor.ViewportId");


FLinearColor FMyAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FName FMyAssetEditor::GetToolkitFName() const
{
	return FName(TEXT("MyAssetEditor"));
}

FText FMyAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("MyAssetEditor", "MyAsset Editor");
}

FString FMyAssetEditor::GetWorldCentricTabPrefix() const 
{
	return LOCTEXT("MyAssetEditorWorldCentricTabPrefix", "MyAsset Editor World Centric Tab Prefix").ToString();
}


void FMyAssetEditor::InitGraphAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost> InToolkitHost, class UMyAsset* InAsset)
{
	MyAssetObj = InAsset;

	/*
	if (!MyAssetObj->EdGraph)
	{
		MyAssetObj->EdGraph = CastChecked<UEdGraph>(FBlueprintEditorUtils::CreateNewGraph(MyAssetObj, NAME_None, UEdGraph::StaticClass(), UEdGraphSchema::StaticClass()));
		//MyAssetObj->EdGraph->bAllowDeletion = bAllowDeletion;
	}
	*/

	EditorViewport = SNew(SMyAssetViewport);


	//FGraphEditorCommands::Register();
	//BindToolkitCommands();

	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(TEXT("Layout_MyAssetEditor"))
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FMyAssetEditor::GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
				->SetSizeCoefficient(0.1f)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FMyAssetEditor::ViewportId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.5f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FMyAssetEditor::GraphTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.1f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FMyAssetEditor::DetailsTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.4f)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(InMode, InToolkitHost, FName("Test MyAsset Editor"), Layout, true, true, InAsset);
}

void FMyAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("MyAssetEditorWorkspaceMenuCategory", "MyAsset Editor WorkspaceMenuCategory"));
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	//InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabDetails))
	//	.SetDisplayName(LOCTEXT("MyAssetEditorDetailsTab", "MyAsset Details"))
	//	.SetGroup(WorkspaceMenuCategoryRef);

	//InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabGraph))
	//	.SetDisplayName(LOCTEXT("MyAssetEditorGraphTab", "MyAsset Graph"))
	//	.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(ViewportId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabViewport))
		.SetDisplayName(LOCTEXT("MyAssetEditorViewportTab", "MyAsset Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

void FMyAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportId);
	//InTabManager->UnregisterTabSpawner(GraphTabId);
	//InTabManager->UnregisterTabSpawner(DetailsTabId);
}

/*
TSharedRef<SDockTab> FMyAssetEditor::HandleSpawnTabDetails(const FSpawnTabArgs& Args)
{
	return HandleSpawnTabGraph(Args);
}

TSharedRef<SDockTab> FMyAssetEditor::HandleSpawnTabGraph(const FSpawnTabArgs& Args)
{
	EdGraphEditor =
		SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.GraphToEdit(MyAssetObj->EdGraph);

	return SNew(SDockTab).TabRole(ETabRole::PanelTab)[EdGraphEditor.ToSharedRef()];
}
*/

TSharedRef<SDockTab> FMyAssetEditor::HandleSpawnTabViewport(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.Label(LOCTEXT("Viewport_TabTitle", "MyAsset Viewport"))
		[
			EditorViewport.ToSharedRef()
		];
			
	return SpawnedTab;
}
#undef LOCTEXT_NAMESPACE