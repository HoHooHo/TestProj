// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorViewportClient_MyAsset.h"
#include "SEditorViewport_MyAsset.h"
#include "AssetViewerSettings.h"

FEditorViewportClient_MyAsset::FEditorViewportClient_MyAsset(const TSharedRef<SEditorViewport_MyAsset>& InViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene)
	: FEditorViewportClient(nullptr, &InPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(InViewport))
{

	SetViewMode(VMI_Lit);

	// Always composite editor objects after post processing in the editor
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	EngineShowFlags.DisableAdvancedFeatures();

	// Setup defaults for the common draw helper.
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(70, 70, 70);
	DrawHelper.GridColorMajor = FColor(40, 40, 40);
	DrawHelper.GridColorMinor = FColor(20, 20, 20);
	DrawHelper.PerspectiveGridSize = HALF_WORLD_MAX1;


	//Initiate view
	SetViewLocation(FVector(0, 0, 75));

	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);
}