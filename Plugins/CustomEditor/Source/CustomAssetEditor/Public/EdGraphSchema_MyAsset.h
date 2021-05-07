// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"
#include "EdGraphSchema_MyAsset.generated.h"

UCLASS(MinimalAPI)
class UEdGraphNode_MyAsset : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UEdGraphNode_MyAsset();
	virtual ~UEdGraphNode_MyAsset();

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString NodeString;
};


USTRUCT()
struct FEdGraphSchemaAction_MyAsset : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

public:
	FEdGraphSchemaAction_MyAsset();
	FEdGraphSchemaAction_MyAsset(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping);

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode  = true) override;

	UEdGraphNode_MyAsset* NodeTemplate;
};

class FConnectionDrawingPolicy_MyAsset : public FConnectionDrawingPolicy
{
protected:
	UEdGraph* EdGraph;
	TMap<UEdGraphNode*, int32> NodeWidgetMap;

public:
	FConnectionDrawingPolicy_MyAsset(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InEdGraph);


	// FConnectionDrawingPolicy interface 
	virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params) override;
	virtual void Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes) override;
	virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
	virtual void DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params) override;
	virtual void DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2D& StartPoint, const FVector2D& EndPoint, UEdGraphPin* Pin) override;
	virtual FVector2D ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const override;
	// End of FConnectionDrawingPolicy interface

protected:
	void Internal_DrawLineWithArrow(const FVector2D& StartAnchorPoint, const FVector2D& EndAnchorPoint, const FConnectionParams& Params);
};

UCLASS(MinimalAPI)
class UEdGraphSchema_MyAsset : public UEdGraphSchema
{
	GENERATED_BODY()
public:

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};