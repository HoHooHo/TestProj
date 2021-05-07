// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Misc/NotifyHook.h"

class UMyAsset;

/**  */
class FMyAssetEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void InitGraphAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost> InToolkitHost, class UMyAsset* InAsset);


private:
	static const FName DetailsTabId;
	static const FName GraphTabId;
	static const FName ViewportId;

	UMyAsset* MyAssetObj;

	TSharedPtr<SEditorViewport> EditorViewport;
	TSharedPtr<IDetailsView> DetailsView;
	/*
	TSharedPtr<SGraphEditor> EdGraphEditor;

	TSharedPtr<FUICommandList> GraphEditorCommands;

	TSharedRef<SDockTab> HandleSpawnTabGraph(const FSpawnTabArgs& Args);
	*/
	TSharedRef<SDockTab> HandleSpawnTabViewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleSpawnTabDetails(const FSpawnTabArgs& Args);
};