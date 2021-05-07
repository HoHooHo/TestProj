// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetEditorToolkit_MyAsset.h"
#include "MyAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "SEditorViewport_MyAsset.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "EdGraph_MyAsset.h"
#include "EdGraphSchema_MyAsset.h"


#define LOCTEXT_NAMESPACE "CustomAssetEditor"

const FName FAssetEditorToolkit_MyAsset::DetailsTabId = TEXT("AssetEditorToolkit_MyAsset.DetailsTabId");
const FName FAssetEditorToolkit_MyAsset::GraphTabId = TEXT("AssetEditorToolkit_MyAsset.GraphTabId");
const FName FAssetEditorToolkit_MyAsset::ViewportId = TEXT("AssetEditorToolkit_MyAsset.ViewportId");


FLinearColor FAssetEditorToolkit_MyAsset::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FName FAssetEditorToolkit_MyAsset::GetToolkitFName() const
{
	return FName(TEXT("AssetEditorToolkit_MyAsset"));
}

FText FAssetEditorToolkit_MyAsset::GetBaseToolkitName() const
{
	return LOCTEXT("AssetEditorToolkit_MyAsset", "MyAsset Editor");
}

FString FAssetEditorToolkit_MyAsset::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("AssetEditorToolkit_MyAssetWorldCentricTabPrefix", "MyAsset Editor World Centric Tab Prefix").ToString();
}


void FAssetEditorToolkit_MyAsset::InitGraphAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost> InToolkitHost, class UMyAsset* InAsset)
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

	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(TEXT("Layout_AssetEditorToolkit_MyAsset"))
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FAssetEditorToolkit_MyAsset::GetToolbarTabId(), ETabState::OpenedTab)
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
					->AddTab(FAssetEditorToolkit_MyAsset::ViewportId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.3f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FAssetEditorToolkit_MyAsset::GraphTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.5f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FAssetEditorToolkit_MyAsset::DetailsTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.2f)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(InMode, InToolkitHost, FName("Test MyAsset Editor"), Layout, true, true, InAsset);
}

void FAssetEditorToolkit_MyAsset::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("AssetEditorToolkit_MyAssetWorkspaceMenuCategory", "MyAsset Editor WorkspaceMenuCategory"));
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ViewportId, FOnSpawnTab::CreateSP(this, &FAssetEditorToolkit_MyAsset::HandleSpawnTabViewport))
		.SetDisplayName(LOCTEXT("AssetEditorToolkit_MyAssetViewportTab", "MyAsset Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FAssetEditorToolkit_MyAsset::HandleSpawnTabGraph))
		.SetDisplayName(LOCTEXT("AssetEditorToolkit_MyAssetGraphTab", "MyAsset Graph"))
		.SetGroup(WorkspaceMenuCategoryRef);

	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FAssetEditorToolkit_MyAsset::HandleSpawnTabDetails))
		.SetDisplayName(LOCTEXT("AssetEditorToolkit_MyAssetDetailsTab", "MyAsset Details"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

void FAssetEditorToolkit_MyAsset::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportId);
	InTabManager->UnregisterTabSpawner(GraphTabId);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
}

TSharedRef<SDockTab> FAssetEditorToolkit_MyAsset::HandleSpawnTabViewport(const FSpawnTabArgs& Args)
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

TSharedRef<SDockTab> FAssetEditorToolkit_MyAsset::HandleSpawnTabGraph(const FSpawnTabArgs& Args)
{
	EdGraphEditor =
		SNew(SGraphEditor)
		//.AdditionalCommands(GraphEditorCommands)
		.GraphToEdit(MyAssetObj->EdGraph);

	return SNew(SDockTab).TabRole(ETabRole::PanelTab)[EdGraphEditor.ToSharedRef()];
}

TSharedRef<SDockTab> FAssetEditorToolkit_MyAsset::HandleSpawnTabDetails(const FSpawnTabArgs& Args)
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

void FAssetEditorToolkit_MyAsset::BindToolkitCommands()
{
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList());

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FAssetEditorToolkit_MyAsset::OnCommandDelete),
			FCanExecuteAction::CreateRaw(this, &FAssetEditorToolkit_MyAsset::CanDeleteNodes)
		);
	}
}

void FAssetEditorToolkit_MyAsset::OnCommandDelete()
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

bool FAssetEditorToolkit_MyAsset::CanDeleteNodes()
{
	return true;
}


#undef LOCTEXT_NAMESPACE