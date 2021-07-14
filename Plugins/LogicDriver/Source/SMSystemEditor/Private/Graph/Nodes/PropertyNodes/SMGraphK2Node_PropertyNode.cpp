// Copyright Recursoft LLC 2019-2020. All Rights Reserved.
#include "SMGraphK2Node_PropertyNode.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SSMGraphProperty.h"
#include "Input/Reply.h"
#include "Components/HorizontalBox.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ScopedTransaction.h"
#include "Graph/Nodes/SMGraphNode_Base.h"
#include "Graph/SMPropertyGraph.h"
#include "Commands/SMEditorCommands.h"
#include "Utilities/SMBlueprintEditorUtils.h"
#include "SMBlueprintEditor.h"
#include "ToolMenu.h"

#define LOCTEXT_NAMESPACE "SMPropertyNode"

USMGraphK2Node_PropertyNode_Base::USMGraphK2Node_PropertyNode_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), OwningGraphNode(nullptr), bMouseOverNodeProperty(false), bDefaultValueChanged(false),
	  bGeneratedDefaultValueBeingSet(false)
{
}

FText USMGraphK2Node_PropertyNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Result", "Result");
}

FText USMGraphK2Node_PropertyNode_Base::GetTooltipText() const
{
	return LOCTEXT("GraphPropertyToolTip", "Evaluates the result for the graph property.");
}

void USMGraphK2Node_PropertyNode_Base::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();

	// Possible this could be run from a state machine state node where there is no run-time container.
	if (USMGraphK2Node_RuntimeNodeContainer* Container = GetRuntimeContainer())
	{
		ContainerOwnerGuid = Container->ContainerOwnerGuid;
		RuntimeNodeGuid = Container->GetRunTimeNodeChecked()->GetNodeGuid();
	}
}

void USMGraphK2Node_PropertyNode_Base::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	if(Pin == GetResultPin())
	{
		// Only the autogenerated default value was changed. Handle it here.
		if (bGeneratedDefaultValueBeingSet)
		{
			return;
		}

		bDefaultValueChanged = true;
	}
	
	Super::PinDefaultValueChanged(Pin);
	SetPropertyDefaultsFromPin();
}

void USMGraphK2Node_PropertyNode_Base::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);
	// Warning. If the graph is being trashed this will cause a check fail.
	//SetPropertyDefaultsFromPin();

	if(Pin && Pin->LinkedTo.Num() > 0)
	{
		// Get all connected nodes because their could be autocast nodes placed as well.
		TSet<UEdGraphNode*> ConnectedNodes;
		FSMBlueprintEditorUtils::GetAllConnectedNodes(Pin->LinkedTo[0]->GetOwningNode(), EGPD_Input, ConnectedNodes);
		for (UEdGraphNode* OwningNode : ConnectedNodes)
		{
			if (USMPropertyGraph* PropertyGraph = GetPropertyGraph())
			{
				/*
				 * What's happening here is a connection is being made across graphs. This is likely
				 * from drag dropping a function DIRECTLY on the pin property value which is technically
				 * in the sm graph. We have handling outside of here to destroy those nodes when placed on the graph
				 * but it tries to forward valid nodes over a property here but can't get them all depending where on
				 * the pin it is dropped. We can salvage the connection here. Unfortunately an ensure is usually
				 * tripped before this point.
				 */
				if (OwningNode->GetGraph() != PropertyGraph)
				{
					OwningNode->Rename(nullptr, PropertyGraph,
						REN_DoNotDirty | REN_DontCreateRedirectors | REN_ForceNoResetLoaders);
					OwningNode->ClearFlags(RF_Transient);
					PropertyGraph->AddNode(OwningNode);
				}
			}
		}
	}
	
	// Pin is changed if user manually places and connects a value or if the value is dropped directly on the exposed k2 pin.
	ForceVisualRefresh();
}

bool USMGraphK2Node_PropertyNode_Base::HasExternalDependencies(TArray<UStruct*>* OptionalOutput) const
{
	const UBlueprint* SourceBlueprint = GetBlueprint();

	USMNodeInstance* OwningTemplate = GetOwningTemplate();
	UClass* SourceClass = OwningTemplate ? OwningTemplate->GetClass() : nullptr;
	const bool bResult = (SourceClass != nullptr) && (SourceClass->ClassGeneratedBy != SourceBlueprint);
	if (bResult && OptionalOutput)
	{
		OptionalOutput->AddUnique(SourceClass);
	}
	const bool bSuperResult = Super::HasExternalDependencies(OptionalOutput);
	return bSuperResult || bResult;
}

void USMGraphK2Node_PropertyNode_Base::PreCompileValidate(FCompilerResultsLog& MessageLog)
{
	Super::PreCompileValidate(MessageLog);

	// Update the runtime node from the editor node.
	ConfigureRuntimePropertyNode();
	
	FSMGraphProperty_Base* GraphProperty = GetPropertyNodeConstChecked();
	if(GraphProperty->ShouldAutoAssignVariable())
	{
		if (UBlueprint* Blueprint = GetTemplateBlueprint())
		{
			FProperty* Property = GraphProperty->MemberReference.ResolveMember<FProperty>(Blueprint);
			if (!Property)
			{
				MessageLog.Error(TEXT("Graph Property Node @@ can't find target property. Please recompile the blueprint @@."), this, Blueprint);
				return;
			}
		}
	}
}

void USMGraphK2Node_PropertyNode_Base::ResetProperty()
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "ResetProperty", "Reset Property"));

	USMPropertyGraph* PropertyGraph = GetPropertyGraph();
	PropertyGraph->SetUsingGraphToEdit(GetPropertyNodeConstChecked()->ShouldDefaultToEditMode());
	PropertyGraph->ResetGraph();
	
	bDefaultValueChanged = false;
	SetPinValueFromPropertyDefaults();
	// Make sure the archetype value is updated.
	SetPropertyDefaultsFromPin();

	GetOwningGraphNode()->ReconstructNode();
}

void USMGraphK2Node_PropertyNode_Base::SetPropertyDefaultsFromPin()
{
	if (USMNodeInstance* Template = GetOwningTemplate())
	{
		if (UEdGraphPin* ResultPin = GetResultPin())
		{
			if (ResultPin->LinkedTo.Num() == 0)
			{
				FSMGraphProperty_Base* GraphProperty = GetPropertyNodeChecked();
				if (FProperty* Property = GraphProperty->MemberReference.ResolveMember<FProperty>(Template->GetClass()))
				{
					if(FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
					{
						/* TODO: Check about proper array default initialization.
						FScriptArrayHelper Helper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<uint8>(Template));
						Helper.GetRawPtr(GraphProperty->ArrayIndex);
						Property->ImportText(*ResultPin->GetDefaultAsString(), Helper.GetRawPtr(GraphProperty->ArrayIndex), 0, Template);
						*/
					}
					else
					{
						Property->ImportText(*ResultPin->GetDefaultAsString(), Property->ContainerPtrToValuePtr<uint8>(Template, GraphProperty->ArrayIndex), 0, Template);
					}
					FSMBlueprintEditorUtils::ConditionallyCompileBlueprint(FSMBlueprintEditorUtils::FindBlueprintForNodeChecked(this));
				}
			}
		}
	}
}

void USMGraphK2Node_PropertyNode_Base::SetPinValueFromPropertyDefaults(bool bUpdateTemplateDefaults)
{
	if(bDefaultValueChanged)
	{
		/*
		 * Assume the pin is accurate and update the default value of the archetype. Pasting nodes doesn't grab the updated value
		 * when a variable name has a special character like `[` or `(` character in it. We think this is a problem with CopyPropertiesForUnrelatedObjects.
		 * Without this code default values may not be set and will require graph evaluation.
		 */
		if (bUpdateTemplateDefaults)
		{
			SetPropertyDefaultsFromPin();
		}
		return;
	}
	
	if (USMNodeInstance* Template = GetOwningTemplate())
	{
		// Ignore REINST classes.
		UClass* Class = Template->GetClass();
		if (Class->GetName().StartsWith(TEXT("REINST_")))
		{
			return;
		}

		// Switch to the CDO so we can get the real defaults.
		Template = CastChecked<USMNodeInstance>(Class->GetDefaultObject());
		
		if (UEdGraphPin* ResultPin = GetResultPin())
		{
			// Only reset if the user hasn't changed the default value.
			if (!bDefaultValueChanged || ResultPin->DefaultValue == ResultPin->AutogeneratedDefaultValue || ResultPin->DefaultValue == LastAutoGeneratedDefaultValue)
			{
				FSMGraphProperty_Base* GraphProperty = GetPropertyNodeChecked();
				if (FProperty* Property = GraphProperty->MemberReference.ResolveMember<FProperty>(Template->GetClass()))
				{
					if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
					{
						/* TODO: Check about proper array default initialization.
						FScriptArrayHelper Helper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<uint8>(Template));
						Helper.GetRawPtr(GraphProperty->ArrayIndex);
						Property->ImportText(*ResultPin->GetDefaultAsString(), Helper.GetRawPtr(GraphProperty->ArrayIndex), 0, Template);
						*/
					}
					else
					{
						uint8* DefaultValue = Property->ContainerPtrToValuePtrForDefaults<uint8>(Template->GetClass(), Template, GraphProperty->ArrayIndex);

						FString TextValue;
						Property->ExportTextItem(TextValue, DefaultValue, DefaultValue, Template, 0);

						// Manually set and track generated default value. Using SetPinAutogeneratedDefaultValue does not work as expected.
						LastAutoGeneratedDefaultValue = TextValue;

						// Special handling in PinDefaultValueChanged which TrySetDefaultValue calls and would be nice to avoid. (It is avoided if SetPinAutogeneratedDefaultValue worked)
						bGeneratedDefaultValueBeingSet = true;
						const UEdGraphSchema_K2* Schema = CastChecked<UEdGraphSchema_K2>(GetSchema());
						Schema->TrySetDefaultValue(*ResultPin, TextValue);
						bGeneratedDefaultValueBeingSet = false;
					}
				}
			}
		}
	}
}

UScriptStruct* USMGraphK2Node_PropertyNode_Base::GetRuntimePropertyNodeType() const
{
	if (FStructProperty* StructProperty = GetRuntimePropertyNodeProperty())
	{
		return StructProperty->Struct;
	}

	return nullptr;
}

FStructProperty* USMGraphK2Node_PropertyNode_Base::GetRuntimePropertyNodeProperty() const
{
	if (FStructProperty* StructProperty = GetPropertyNodeProperty(true))
	{
		return StructProperty;
	}

	return GetPropertyNodeProperty(false);
}

FStructProperty* USMGraphK2Node_PropertyNode_Base::GetPropertyNodeProperty(bool bRuntimeOnly) const
{
	UScriptStruct* BaseFStruct = FSMGraphProperty_Base_Runtime::StaticStruct();
	UScriptStruct* ExcludeClass = FSMGraphProperty_Base::StaticStruct();

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct->IsChildOf(BaseFStruct) && (!bRuntimeOnly || !StructProp->Struct->IsChildOf(ExcludeClass)))
			{
				return StructProp;
			}
		}
	}

	return nullptr;
}

USMNodeInstance* USMGraphK2Node_PropertyNode_Base::GetOwningTemplate() const
{
	if(!OwningGraphNode)
	{
		return nullptr;
	}

	FSMGraphProperty_Base* PropertyNode = GetPropertyNodeConstChecked();
	if (USMNodeInstance* NodeInstance = OwningGraphNode->GetNodeTemplateFromGuid(PropertyNode->GetGuid()))
	{
		return NodeInstance;
	}

	if (PropertyNode->GetTemplateGuid().IsValid())
	{
		/*
		 * This is a stack template but the template could not be found!
		 * This *should* only occur when copy & pasting and the stack hasn't fully regenerated.
		 */
		return nullptr;
	}

	// Fallback to original behavior since the guid template tracking won't be setup on < 2.4 imports.
	return OwningGraphNode->GetNodeTemplate();
}

USMNodeBlueprint* USMGraphK2Node_PropertyNode_Base::GetTemplateBlueprint() const
{
	if (USMNodeInstance* Instance = GetOwningTemplate())
	{
		return Cast<USMNodeBlueprint>(UBlueprint::GetBlueprintFromClass(Instance->GetClass()));
	}

	return nullptr;
}

UEdGraph* USMGraphK2Node_PropertyNode_Base::GetOwningGraph() const
{
	return OwningGraphNode ? OwningGraphNode->GetGraphPropertyGraph(const_cast<USMGraphK2Node_PropertyNode_Base*>(this)->GetPropertyNodeChecked()->GetGuid()) : nullptr;
}

USMGraphNode_Base* USMGraphK2Node_PropertyNode_Base::GetOwningGraphNode() const
{
	return OwningGraphNode;
}

void USMGraphK2Node_PropertyNode_Base::JumpToPropertyGraph()
{
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GetPropertyGraph());
}

void USMGraphK2Node_PropertyNode_Base::JumpToTemplateBlueprint()
{
	if (UBlueprint* TemplateBlueprint = GetTemplateBlueprint())
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(TemplateBlueprint);
	}
}

TSharedPtr<SSMGraphProperty_Base> USMGraphK2Node_PropertyNode_Base::GetGraphNodeWidget() const
{
	return nullptr;
}

TSharedPtr<SWidget> USMGraphK2Node_PropertyNode_Base::GetGraphDetailWidget() const
{
	TSharedRef<SHorizontalBox> DetailWidget = SNew(SHorizontalBox);
	DetailWidget->AddSlot()
	.AutoWidth()
	[
		GetViewGraphDetailWidget().ToSharedRef()
	];

	if (GetPropertyNodeConstChecked()->AllowToggleGraphEdit())
	{
		DetailWidget->AddSlot()
		.AutoWidth()
		[
			GetToggleEditGraphDetailWidget().ToSharedRef()
		];
	}

	return DetailWidget;
}

TSharedPtr<SWidget> USMGraphK2Node_PropertyNode_Base::GetViewGraphDetailWidget() const
{
	UEdGraph* Graph = GetOwningGraph();
	check(Graph);
	const FText EditGraphText = Graph->bEditable ? LOCTEXT("EditGraphProperty", "Edit Graph") : LOCTEXT("ViewGraphProperty", "View Graph");
	
	return SNew(SButton)
		.Text(EditGraphText)
		.OnClicked_Lambda([this]()
		{
			if (UEdGraph* Graph = this->GetOwningGraph())
			{
				FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(Graph);
			}
			return FReply::Handled();
		});
}

TSharedPtr<SWidget> USMGraphK2Node_PropertyNode_Base::GetToggleEditGraphDetailWidget() const
{
	USMPropertyGraph* Graph = Cast<USMPropertyGraph>(GetOwningGraph());
	check(Graph);
	const FText EditGraphText = Graph->IsGraphBeingUsedToEdit() ? LOCTEXT("DisableGraphEdit", "Revert to Node Edit") : LOCTEXT("EnableGraphEdit", "Convert to Graph Edit");

	return SNew(SButton)
		.Text(EditGraphText)
		.OnClicked_Lambda([this]()
		{
			if (USMPropertyGraph* Graph = Cast<USMPropertyGraph>(this->GetOwningGraph()))
			{
				Graph->ToggleGraphPropertyEdit();
			}
			return FReply::Handled();
		});
}

void USMGraphK2Node_PropertyNode_Base::GetContextMenuActionsForOwningNode(const UEdGraph* CurrentGraph,
                                                                     const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class UToolMenu* ToolMenu, bool bIsDebugging) const
{
	// This flag is normally cleared unless a right mouse click was detected.
	const_cast<USMGraphK2Node_PropertyNode_Base*>(this)->bMouseOverNodeProperty = false;
	
	FSMBlueprintEditor* Editor = FSMBlueprintEditorUtils::GetStateMachineEditor(this);
	check(Editor);
	Editor->SelectedPropertyNode = const_cast<USMGraphK2Node_PropertyNode_Base*>(this);

	FSMGraphProperty_Base* GraphProperty = GetPropertyNodeConst();
	check(GraphProperty);
	
	FToolMenuSection& Section = ToolMenu->AddSection("SMPropertyGraphSchemaActions", GraphProperty->GetDisplayName());
	{
		Internal_GetContextMenuActionsForOwningNode(CurrentGraph, InGraphNode, InGraphPin, Section, bIsDebugging);
	}
}

UEdGraphPin* USMGraphK2Node_PropertyNode_Base::GetResultPin() const
{
	if(UEdGraphPin* Pin = FindPin(GetPropertyNodeConstChecked()->VariableName))
	{
		return Pin;
	}
	
	return FindPin(TEXT("Result"));
}

void USMGraphK2Node_PropertyNode_Base::Internal_GetContextMenuActionsForOwningNode(const UEdGraph* CurrentGraph,
	const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, FToolMenuSection& MenuSection, bool bIsDebugging) const
{
	MenuSection.AddMenuEntry(FSMEditorCommands::Get().GoToPropertyGraph);

	if (!bIsDebugging)
	{
		if (GetPropertyGraph()->IsGraphBeingUsedToEdit())
		{
			MenuSection.AddMenuEntry(FSMEditorCommands::Get().RevertPropertyToNodeEdit);
		}
		else
		{
			MenuSection.AddMenuEntry(FSMEditorCommands::Get().ConvertPropertyToGraphEdit);
		}

		MenuSection.AddMenuEntry(FSMEditorCommands::Get().ResetGraphProperty);
	}
}

#undef LOCTEXT_NAMESPACE