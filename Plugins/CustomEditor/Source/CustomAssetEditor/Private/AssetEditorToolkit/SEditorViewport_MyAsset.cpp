// Copyright Epic Games, Inc. All Rights Reserved.

#include "SEditorViewport_MyAsset.h"
#include "EditorViewportClient_MyAsset.h"

SEditorViewport_MyAsset::SEditorViewport_MyAsset()
	: PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{

}

SEditorViewport_MyAsset::~SEditorViewport_MyAsset()
{
	if (EditorViewportClient.IsValid())
	{
		EditorViewportClient->Viewport = nullptr;
	}
}

void SEditorViewport_MyAsset::Construct(const FArguments& InArgs)
{
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SEditorViewport_MyAsset::AddReferencedObjects(FReferenceCollector& Collector)
{

}

TSharedRef<SEditorViewport> SEditorViewport_MyAsset::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SEditorViewport_MyAsset::GetExtenders() const
{
	return MakeShareable(new FExtender());
}

void SEditorViewport_MyAsset::OnFloatingButtonClicked()
{

}

TSharedRef<FEditorViewportClient> SEditorViewport_MyAsset::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FEditorViewportClient_MyAsset(SharedThis(this), PreviewScene.ToSharedRef()));

	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<FEditorViewportClient> SEditorViewport_MyAsset::GetViewportClient() const
{
	return EditorViewportClient;
}