// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyAssetEditor.h"
#include "MyAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "SEditorViewport_MyAsset.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "EdGraph_MyAsset.h"
#include "EdGraphSchema_MyAsset.h"


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

	if (!MyAssetObj->EdGraph)
	{
		MyAssetObj->EdGraph = CastChecked<UEdGraph_MyAsset>(FBlueprintEditorUtils::CreateNewGraph(MyAssetObj, NAME_None, UEdGraph_MyAsset::StaticClass(), UEdGraphSchema_MyAsset::StaticClass()));
		MyAssetObj->EdGraph->bAllowDeletion = false;
	}

	EditorViewport = SNew(SEditorViewport_MyAsset);


	FGraphEditorCommands::Register();
	BindToolkitCommands();

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
					->SetSizeCoefficient(0.3f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FMyAssetEditor::GraphTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.5f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FMyAssetEditor::DetailsTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.2f)
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

	InTabManager->RegisterTabSpawner(ViewportId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabViewport))
		.SetDisplayName(LOCTEXT("MyAssetEditorViewportTab", "MyAsset Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabGraph))
		.SetDisplayName(LOCTEXT("MyAssetEditorGraphTab", "MyAsset Graph"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FMyAssetEditor::HandleSpawnTabDetails))
		.SetDisplayName(LOCTEXT("MyAssetEditorDetailsTab", "MyAsset Details"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

void FMyAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportId);
	InTabManager->UnregisterTabSpawner(GraphTabId);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
}

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

TSharedRef<SDockTab> FMyAssetEditor::HandleSpawnTabGraph(const FSpawnTabArgs& Args)
{
	EdGraphEditor =
		SNew(SGraphEditor)
		//.AdditionalCommands(GraphEditorCommands)
		.GraphToEdit(MyAssetObj->EdGraph);

	return SNew(SDockTab).TabRole(ETabRole::PanelTab)[EdGraphEditor.ToSharedRef()];
}

TSharedRef<SDockTab> FMyAssetEditor::HandleSpawnTabDetails(const FSpawnTabArgs& Args)
{
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = true;
	DetailsViewArgs.bCustomNameAreaLocation = true;
	DetailsViewArgs.bLockable = false;
	//DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.NotifyHook = this;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(MyAssetObj);

	return SNew(SDockTab).TabRole(ETabRole::PanelTab)[DetailsView.ToSharedRef()];
}

void FMyAssetEditor::BindToolkitCommands()
{
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList());

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FMyAssetEditor::OnCommandDelete),
			FCanExecuteAction::CreateRaw(this, &FMyAssetEditor::CanDeleteNodes)
		);
	}
}

void FMyAssetEditor::OnCommandDelete()
{
	EdGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectdNodes = EdGraphEditor->GetSelectedNodes();
	EdGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator It(SelectdNodes); It; ++It)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*It))
		{
			Node->Modify();
			Node->DestroyNode();
		}
	}
}

bool FMyAssetEditor::CanDeleteNodes()
{
	return true;
}


#undef LOCTEXT_NAMESPACE