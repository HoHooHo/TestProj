// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMyAssetViewport.h"
#include "MyAssetViewportClient.h"

SMyAssetViewport::SMyAssetViewport()
	: PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{

}

SMyAssetViewport::~SMyAssetViewport()
{
	if (EditorViewportClient.IsValid())
	{
		EditorViewportClient->Viewport = nullptr;
	}
}

void SMyAssetViewport::Construct(const FArguments& InArgs)
{
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SMyAssetViewport::AddReferencedObjects(FReferenceCollector& Collector)
{

}

TSharedRef<SEditorViewport> SMyAssetViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SMyAssetViewport::GetExtenders() const
{
	return MakeShareable(new FExtender());
}

void SMyAssetViewport::OnFloatingButtonClicked()
{

}

TSharedRef<FEditorViewportClient> SMyAssetViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FMyAssetViewportClient(SharedThis(this), PreviewScene.ToSharedRef()));

	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<FEditorViewportClient> SMyAssetViewport::GetViewportClient() const
{
	return EditorViewportClient;
}