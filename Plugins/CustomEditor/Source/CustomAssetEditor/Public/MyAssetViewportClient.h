// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "AdvancedPreviewScene.h"

class SMyAssetViewport;

class FMyAssetViewportClient : public FEditorViewportClient
{
public:
	/** Pointer back to the MyAsset viewport control that owns us */
	TWeakPtr<SMyAssetViewport> ViewportPtr;

	FMyAssetViewportClient(const TSharedRef<SMyAssetViewport>& InViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene);

	/** Stored pointer to the preview scene in which the static mesh is shown */
	FAdvancedPreviewScene* AdvancedPreviewScene;
};