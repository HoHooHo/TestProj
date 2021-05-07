// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "AdvancedPreviewScene.h"

class SEditorViewport_MyAsset;

class FEditorViewportClient_MyAsset : public FEditorViewportClient
{
public:
	/** Pointer back to the MyAsset viewport control that owns us */
	TWeakPtr<SEditorViewport_MyAsset> ViewportPtr;

	FEditorViewportClient_MyAsset(const TSharedRef<SEditorViewport_MyAsset>& InViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene);

	/** Stored pointer to the preview scene in which the static mesh is shown */
	FAdvancedPreviewScene* AdvancedPreviewScene;
};