// Copyright SinceTimes, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Ability Details Tab
class SDetailsViewTabBody_MyBlueprint : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDetailsViewTabBody_MyBlueprint) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class FAssetEditorToolkit_MyBlueprint> InAssetEditor);

private:
	TSharedPtr<class IDetailsView> PropertyView;
};