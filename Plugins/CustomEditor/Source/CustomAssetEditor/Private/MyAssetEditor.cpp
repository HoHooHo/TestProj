// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyAssetEditor.h"
#include "MyAsset.h"


#define LOCTEXT_NAMESPACE "CustomAssetEditor"


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
	FAssetEditorToolkit::InitAssetEditor(InMode, InToolkitHost, FName("Test MyAsset Editor"), FTabManager::FLayout::NullLayout, true, true, InAsset);
}

#undef LOCTEXT_NAMESPACE