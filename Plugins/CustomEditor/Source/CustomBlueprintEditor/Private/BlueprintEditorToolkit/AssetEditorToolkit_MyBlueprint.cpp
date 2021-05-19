// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEditorToolkit_MyBlueprint.h"
#include "EdGraphSchema_K2_MyBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"


#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"


void FAssetEditorToolkit_MyBlueprint::InitEditor_MyBlueprint(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	FBlueprintEditor::InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
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