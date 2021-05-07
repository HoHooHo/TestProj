// Copyright Epic Games, Inc. All Rights Reserved.

#include "EdGraphSchema_MyAsset.h"
#include "ConnectionDrawingPolicy.h"

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

UEdGraphNode_MyAsset::UEdGraphNode_MyAsset()
{
	bCanRenameNode = true;
}

UEdGraphNode_MyAsset::~UEdGraphNode_MyAsset()
{

}

void UEdGraphNode_MyAsset::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

FText UEdGraphNode_MyAsset::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("MyAsset"));
}



FEdGraphSchemaAction_MyAsset::FEdGraphSchemaAction_MyAsset()
	: NodeTemplate(nullptr)
{

}

FEdGraphSchemaAction_MyAsset::FEdGraphSchemaAction_MyAsset(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
	: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
	, NodeTemplate(nullptr)
{

}

UEdGraphNode* FEdGraphSchemaAction_MyAsset::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /* = true */)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate)
	{
		//const FScopedTransaction Transaction(LOCTEXT("GenericGraphEditorNewNode", "Generic Graph Editor: New Node"));
		ParentGraph->Modify();

		if (FromPin)
		{
			FromPin->Modify();
		}

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

FConnectionDrawingPolicy_MyAsset::FConnectionDrawingPolicy_MyAsset(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InEdGraph)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements)
	, EdGraph(InEdGraph)
{

}

void FConnectionDrawingPolicy_MyAsset::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	Params.AssociatedPin1 = OutputPin;
	Params.AssociatedPin2 = InputPin;
	Params.WireThickness = 1.5f;

	const bool bDeemphasizeUnhoveredPins = HoveredPins.Num() > 0;
	if (bDeemphasizeUnhoveredPins)
	{
		ApplyHoverDeemphasis(OutputPin, InputPin, Params.WireThickness, Params.WireColor);
	}
}

void FConnectionDrawingPolicy_MyAsset::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes)
{
	NodeWidgetMap.Empty();

	for (int32 i = 0; i < ArrangedNodes.Num(); ++i)
	{
		FArrangedWidget& CurWidget = ArrangedNodes[i];
		TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);
		NodeWidgetMap.Add(ChildNode->GetNodeObj(), i);
	}

	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

void FConnectionDrawingPolicy_MyAsset::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params)
{
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	DrawSplineWithArrow(StartAnchorPoint, EndAnchorPoint, Params);
}

void FConnectionDrawingPolicy_MyAsset::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params)
{
	// bUserFlag1 indicates that we need to reverse the direction of connection (used by debugger)
	const FVector2D& P0 = Params.bUserFlag1 ? EndPoint : StartPoint;
	const FVector2D& P1 = Params.bUserFlag1 ? StartPoint : EndPoint;

	Internal_DrawLineWithArrow(P0, P1, Params);
}

void FConnectionDrawingPolicy_MyAsset::DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2D& StartPoint, const FVector2D& EndPoint, UEdGraphPin* Pin)
{
	FConnectionParams Params;
	DetermineWiringStyle(Pin, nullptr, Params);

	if (EEdGraphPinDirection::EGPD_Output == Pin->Direction)
	{
		DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, EndPoint), EndPoint, Params);
	}
	else
	{
		DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, StartPoint), StartPoint, Params);
	}
}

FVector2D FConnectionDrawingPolicy_MyAsset::ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const
{
	const FVector2D Delta = End - Start;
	const FVector2D NormalDelta = Delta.GetSafeNormal();

	return NormalDelta;
}

void FConnectionDrawingPolicy_MyAsset::Internal_DrawLineWithArrow(const FVector2D& StartAnchorPoint, const FVector2D& EndAnchorPoint, const FConnectionParams& Params)
{
	//@TODO: Should this be scaled by zoom factor?
	const float LineSeparationAmount = 4.5f;

	const FVector2D DeltaPos = EndAnchorPoint - StartAnchorPoint;
	const FVector2D UnitDelta = DeltaPos.GetSafeNormal();
	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D DirectionBias = Normal * LineSeparationAmount;
	const FVector2D LengthBias = ArrowRadius.X * UnitDelta;
	const FVector2D StartPoint = StartAnchorPoint + DirectionBias + LengthBias;
	const FVector2D EndPoint = EndAnchorPoint + DirectionBias - LengthBias;

	DrawConnection(WireLayerID, StartPoint, EndPoint, Params);

	const FVector2D ArrowDrawPos = EndPoint - ArrowRadius;
	const float AngleInRadians = FMath::Atan2(DeltaPos.Y, DeltaPos.X);

	FSlateDrawElement::MakeRotatedBox
	(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(ArrowDrawPos, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
		ArrowImage,
		ESlateDrawEffect::None,
		AngleInRadians,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		Params.WireColor
	);
}


const FPinConnectionResponse UEdGraphSchema_MyAsset::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!(A && B))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both pins must be available!"));
	}
	else if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect a node to itself!"));
	}
	else if (EGPD_Input == A->Direction && EGPD_Input == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect an input pin to another input pin!"));
	}
	else if (EGPD_Output == A->Direction && EGPD_Output == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect an output pin to another output pin"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UEdGraphSchema_MyAsset::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FText AddToolTip = LOCTEXT("NewGenericGraphNodeTooltip", "Add node here");
	FText Desc = FText::FromString(TEXT("MyAsset Graph Schema Desc"));

	TSharedPtr<FEdGraphSchemaAction_MyAsset> NewNodeAction = MakeShareable(new FEdGraphSchemaAction_MyAsset(LOCTEXT("GenericGraphNodeAction", "Generic Graph Node"), Desc, AddToolTip, 0));
	NewNodeAction->NodeTemplate = NewObject<UEdGraphNode_MyAsset>(ContextMenuBuilder.OwnerOfTemporaries);

	ContextMenuBuilder.AddAction(NewNodeAction);
}

FConnectionDrawingPolicy* UEdGraphSchema_MyAsset::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FConnectionDrawingPolicy_MyAsset(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

#undef LOCTEXT_NAMESPACE