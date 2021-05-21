// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditorModes.h"

/**
 * This is the list of IDs for MyBlueprint Editor modes
 */
struct FApplicationModes_MyBlueprint
{
public:
	// Mode ID constants
	static const FName ModeID1;
	static const FName ModeID2;

	static FText GetLocalizedMode(const FName InMode)
	{
		static TMap< FName, FText > LocModes;

		if (LocModes.Num() == 0)
		{
			LocModes.Add(ModeID1, NSLOCTEXT("CustomBlueprintEditor", "Mode1", "Mode1"));
			LocModes.Add(ModeID2, NSLOCTEXT("CustomBlueprintEditor", "Mode2", "Mode2"));
		}

		check(InMode != NAME_None);
		const FText* OutMode = LocModes.Find(InMode);
		check(OutMode);

		return *OutMode;
	}
};

class FApplicationMode_MyBlueprint_Mode1 : public FApplicationMode
{
public:
	FApplicationMode_MyBlueprint_Mode1(TSharedPtr<class FAssetEditorToolkit_MyBlueprint> InAssetEditorToolkit, FName InModeName, FText(*GetLocalizedMode)(const FName), const bool bRegisterViewport = true, const bool bRegisterDefaultsTab = true);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FAssetEditorToolkit_MyBlueprint> AssetEditorToolkit;

	// Set of spawnable tabs in editing mode
	FWorkflowAllowedTabSet EditorTabFactories;

	// Set of spawnable tabs useful in derived classes, even without a blueprint
	FWorkflowAllowedTabSet CoreTabFactories;

	// Set of spawnable tabs only usable in blueprint editing mode (not useful in Persona, etc...)
	FWorkflowAllowedTabSet EditorOnlyTabFactories;
};


class FApplicationMode_MyBlueprint_Mode2 : public FBlueprintEditorApplicationMode
{
public:
	FApplicationMode_MyBlueprint_Mode2(TSharedPtr<class FAssetEditorToolkit_MyBlueprint> InAssetEditorToolkit, FName InModeName, FText(*GetLocalizedMode)(const FName), const bool bRegisterViewport = true, const bool bRegisterDefaultsTab = true);
};