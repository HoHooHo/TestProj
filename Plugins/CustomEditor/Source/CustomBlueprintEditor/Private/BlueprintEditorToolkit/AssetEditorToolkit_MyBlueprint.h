// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"

/**
 * 
 */
class FAssetEditorToolkit_MyBlueprint : public FBlueprintEditor
{
public:
	/**
	 * Edits the specified blueprint
	 *
	 * @param	Mode					Asset editing mode for this editor (standalone or world-centric)
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	InBlueprints			The blueprints to edit
	 * @param	bShouldOpenInDefaultsMode	If true, the editor will open in defaults editing mode
	 */
	void InitEditor_MyBlueprint(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);

private:
	/** Menu Extension Helper. */
	void ExtendMenu();

public:
	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	//~ End IToolkit Interface

private:
	TSharedPtr<FExtender> MenuExtender;
	TSharedPtr<FExtender> ToolbarExtender;
};
