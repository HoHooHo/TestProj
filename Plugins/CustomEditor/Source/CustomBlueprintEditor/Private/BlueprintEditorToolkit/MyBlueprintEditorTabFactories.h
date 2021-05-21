// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

class FDetailsTabSummoner_MyBlueprint : public FWorkflowTabFactory
{
public:
	static const FName ID;

public:
	FDetailsTabSummoner_MyBlueprint(TSharedPtr<class FAssetEditorToolkit> InHostingApp);

	// FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

	//virtual TSharedPtr<SToolTip> CreateTabToolTipWidget(const FWorkflowTabSpawnInfo& Info) const override;
	// FWorkflowTabFactory interface
};

#undef LOCTEXT_NAMESPACE
