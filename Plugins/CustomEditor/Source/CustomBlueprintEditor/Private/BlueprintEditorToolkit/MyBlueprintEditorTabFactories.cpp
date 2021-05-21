// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyBlueprintEditorTabFactories.h"
#include "CustomBlueprintStyle.h"
#include "Slate/SDetailsViewTabBody_MyBlueprint.h"
#include "AssetEditorToolkit_MyBlueprint.h"

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"


const FName FDetailsTabSummoner_MyBlueprint::ID(TEXT("AssetEditorToolkit_MyBlueprint_DetailsTabSummoner"));

FDetailsTabSummoner_MyBlueprint::FDetailsTabSummoner_MyBlueprint(TSharedPtr<class FAssetEditorToolkit> InHostingApp)
	: FWorkflowTabFactory(FDetailsTabSummoner_MyBlueprint::ID, InHostingApp)
{
	TabLabel = LOCTEXT("DetailsTabSummonerTabTitle", "MyDetail Tab Title");
	TabIcon = FSlateIcon(FCustomBlueprintStyle::GetStyleSetName(), "CustomeBlueprint.DetailsTabIcon");

	bIsSingleton = false;

	ViewMenuDescription = LOCTEXT("DetailsTabView", "MyDetails Tab View");
	ViewMenuTooltip = LOCTEXT("DetailsTabView_ToolTip", "MyDetails Tab View ToolTip");
}

TSharedRef<SWidget> FDetailsTabSummoner_MyBlueprint::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FAssetEditorToolkit_MyBlueprint> Editor = StaticCastSharedPtr<FAssetEditorToolkit_MyBlueprint>(HostingApp.Pin());
	
	return SNew(SDetailsViewTabBody_MyBlueprint, Editor);
}

//TSharedPtr<SToolTip> FDetailsTabSummoner_MyBlueprint::CreateTabToolTipWidget(const FWorkflowTabSpawnInfo& Info) const
//{
//	const FString DocLink = TEXT("Shared/Editors/Common/EditorTabs");
//	const FString DocExcerptName = TabIdentifier.ToString();
//	return IDocumentation::Get()->CreateToolTip(GetTabToolTipText(Info), NULL, DocLink, DocExcerptName);
//}

#undef LOCTEXT_NAMESPACE
