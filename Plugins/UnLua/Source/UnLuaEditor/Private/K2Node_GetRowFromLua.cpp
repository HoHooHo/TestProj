// Copyright Epic Games, Inc. All Rights Reserved.

#include "K2Node_GetRowFromLua.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "DataTableEditorUtils.h"
#include "UnLuaBlueprintLibrary.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"

#define LOCTEXT_NAMESPACE "K2Node_GetRowFromLua"

namespace GetRowFromLuaHelper
{

const FName TargetStructPinName = "TargetStruct";
const FName LuaTableNamePinName = "LuaTableName";
const FName RowNamePinName = "RowName";
const FName RowNotFoundPinName = "RowNotFound";

}

UK2Node_GetRowFromLua::UK2Node_GetRowFromLua(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Attempts to retrieve a TableRow from a TargetStruct via it's RowName");
}

void UK2Node_GetRowFromLua::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* RowFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	RowFoundPin->PinFriendlyName = LOCTEXT("GetRowFromLua Row Found Exec pin", "Row Found");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetRowFromLuaHelper::RowNotFoundPinName);

	// Add TargetStruct pin
	UEdGraphPin* TargetStructPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UScriptStruct::StaticClass(), GetRowFromLuaHelper::TargetStructPinName);
	SetPinToolTip(*TargetStructPin, LOCTEXT("TargetStructPinDescription", "The TargetStruct you want to retreive a row from"));

	// LuaTable Name pin
	UEdGraphPin* LuaTableNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetRowFromLuaHelper::LuaTableNamePinName);
	SetPinToolTip(*LuaTableNamePin, LOCTEXT("LuaTableNamePinDescription", "The name of the lua-table to retrieve from the TargetStruct"));

	// Row Name pin
	UEdGraphPin* RowNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetRowFromLuaHelper::RowNamePinName);
	SetPinToolTip(*RowNamePin, LOCTEXT("RowNamePinDescription", "The name of the row to retrieve from the TargetStruct"));

	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("GetRowFromLua Output Row", "Out Row");
	SetPinToolTip(*ResultPin, LOCTEXT("ResultPinDescription", "The returned TableRow, if found"));

	Super::AllocateDefaultPins();
}

void UK2Node_GetRowFromLua::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}

	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UK2Node_GetRowFromLua::RefreshOutputPinType()
{
	UScriptStruct* OutputType = GetRowFromLuaStructType();
	SetReturnTypeForStruct(OutputType);
}

void UK2Node_GetRowFromLua::RefreshRowNameOptions()
{
	// When the TargetStruct pin gets a new value assigned, we need to update the Slate UI so that SGraphNodeCallParameterCollectionFunction will update the ParameterName drop down
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}


void UK2Node_GetRowFromLua::SetReturnTypeForStruct(UScriptStruct* NewRowStruct)
{
	UScriptStruct* OldRowStruct = GetReturnTypeForStruct();
	if (NewRowStruct != OldRowStruct)
	{
		UEdGraphPin* ResultPin = GetResultPin();

		if (ResultPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(ResultPin);
		}

		// NOTE: purposefully not disconnecting the ResultPin (even though it changed type)... we want the user to see the old
		//       connections, and incompatible connections will produce an error (plus, some super-struct connections may still be valid)
		ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
		ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;

		CachedNodeTitle.Clear();
	}
}

UScriptStruct* UK2Node_GetRowFromLua::GetReturnTypeForStruct()
{
	UScriptStruct* ReturnStructType = (UScriptStruct*)(GetResultPin()->PinType.PinSubCategoryObject.Get());

	return ReturnStructType;
}

UScriptStruct* UK2Node_GetRowFromLua::GetRowFromLuaStructType() const
{
	UScriptStruct* RowStructType = nullptr;

	UEdGraphPin* TargetStructPin = GetTargetStructPin();
	if(TargetStructPin && TargetStructPin->DefaultObject != nullptr && TargetStructPin->LinkedTo.Num() == 0)
	{
		if (UScriptStruct* TargetStruct = Cast<UScriptStruct>(TargetStructPin->DefaultObject))
		{
			//RowStructType = TargetStruct->RowStruct;
			RowStructType = TargetStruct;
		}
	}

	if (RowStructType == nullptr)
	{
		UEdGraphPin* ResultPin = GetResultPin();
		if (ResultPin && ResultPin->LinkedTo.Num() > 0)
		{
			RowStructType = Cast<UScriptStruct>(ResultPin->LinkedTo[0]->PinType.PinSubCategoryObject.Get());
			for (int32 LinkIndex = 1; LinkIndex < ResultPin->LinkedTo.Num(); ++LinkIndex)
			{
				UEdGraphPin* Link = ResultPin->LinkedTo[LinkIndex];
				UScriptStruct* LinkType = Cast<UScriptStruct>(Link->PinType.PinSubCategoryObject.Get());

				if (RowStructType->IsChildOf(LinkType))
				{
					RowStructType = LinkType;
				}
			}
		}
	}
	return RowStructType;
}

void UK2Node_GetRowFromLua::OnTargetStructRowListChanged(const UScriptStruct* TargetStruct)
{
	UEdGraphPin* TargetStructPin = GetTargetStructPin();
	if (TargetStruct && TargetStructPin && TargetStruct == TargetStructPin->DefaultObject)
	{
		UEdGraphPin* RowNamePin = GetRowNamePin();
		const bool TryRefresh = RowNamePin && !RowNamePin->LinkedTo.Num();
		//const FName CurrentName = RowNamePin ? FName(*RowNamePin->GetDefaultAsString()) : NAME_None;
		//if (TryRefresh && RowNamePin && !TargetStruct->GetRowNames().Contains(CurrentName))
		if (TryRefresh && RowNamePin)
		{
			if (UBlueprint* BP = GetBlueprint())
			{
				FBlueprintEditorUtils::MarkBlueprintAsModified(BP);
			}
		}
	}
}

void UK2Node_GetRowFromLua::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) 
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	if (UEdGraphPin* TargetStructPin = GetTargetStructPin(&OldPins))
	{
		if (UScriptStruct* TargetStruct = Cast<UScriptStruct>(TargetStructPin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetRowFromLuaStructType)
			PreloadObject(TargetStruct);
		}
	}
}

void UK2Node_GetRowFromLua::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_GetRowFromLua::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

bool UK2Node_GetRowFromLua::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetResultPin() && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		bool bDisallowed = true;
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			if (UScriptStruct* ConnectionType = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get()))
			{
				bDisallowed = !FDataTableEditorUtils::IsValidTableStruct(ConnectionType);
			}
		}
		else if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
		{
			bDisallowed = false;
		}

		if (bDisallowed)
		{
			OutReason = TEXT("Must be a struct that can be used in a TargetStruct");
		}
		return bDisallowed;
	}
	return false;
}

void UK2Node_GetRowFromLua::PinDefaultValueChanged(UEdGraphPin* ChangedPin) 
{
	if (ChangedPin && ChangedPin->PinName == GetRowFromLuaHelper::TargetStructPinName)
	{
		RefreshOutputPinType();

		//UEdGraphPin* RowNamePin = GetRowNamePin();
		//UScriptStruct*  TargetStruct = Cast<UScriptStruct>(ChangedPin->DefaultObject);
		//if (RowNamePin)
		//{
		//	//if (TargetStruct && (RowNamePin->DefaultValue.IsEmpty() || !TargetStruct->GetRowMap().Contains(*RowNamePin->DefaultValue)))
		//	{
		//		if (auto Iterator = TargetStruct->GetRowMap().CreateConstIterator())
		//		{
		//			RowNamePin->DefaultValue = Iterator.Key().ToString();
		//		}
		//	}	

		//	RefreshRowNameOptions();
		//}
	}
}

FText UK2Node_GetRowFromLua::GetTooltipText() const
{
	return NodeTooltip;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetThenPin()const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetTargetStructPin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
    
	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == GetRowFromLuaHelper::TargetStructPinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetLuaTableNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetRowFromLuaHelper::LuaTableNamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetRowNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetRowFromLuaHelper::RowNamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetRowNotFoundPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetRowFromLuaHelper::RowNotFoundPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetRowFromLua::GetResultPin() const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

FText UK2Node_GetRowFromLua::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("ListViewTitle", "Get Row from Lua");
	}
	else if (UEdGraphPin* TargetStructPin = GetTargetStructPin())
	{
		if (TargetStructPin->LinkedTo.Num() > 0)
		{
			return NSLOCTEXT("K2Node", "TargetStruct_Title_Unknown", "Get Row from Lua");
		}
		else if (TargetStructPin->DefaultObject == nullptr)
		{
			return NSLOCTEXT("K2Node", "TargetStruct_Title_None", "Get Row from Lua | NONE");
		}
		else if (CachedNodeTitle.IsOutOfDate(this))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("TargetStructName"), FText::FromString(TargetStructPin->DefaultObject->GetName()));

			FText LocFormat = NSLOCTEXT("K2Node", "TargetStruct", "Get Row from Lua | {TargetStructName}");
			// FText::Format() is slow, so we cache this to save on performance
			CachedNodeTitle.SetCachedText(FText::Format(LocFormat, Args), this);
		}
	}
	else
	{
		return NSLOCTEXT("K2Node", "TargetStruct_Title_None", "Get Row from Lua | NONE");
	}	
	return CachedNodeTitle;
}

void UK2Node_GetRowFromLua::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);
    
    UEdGraphPin* OriginalTargetStructInPin = GetTargetStructPin();
    UScriptStruct* Struct = (OriginalTargetStructInPin != NULL) ? Cast<UScriptStruct>(OriginalTargetStructInPin->DefaultObject) : NULL;
    if((nullptr == OriginalTargetStructInPin) || (0 == OriginalTargetStructInPin->LinkedTo.Num() && nullptr == Struct))
    {
        CompilerContext.MessageLog.Error(*LOCTEXT("GetRowFromLuaNoTargetStruct_Error", "GetRowFromLua must have a TargetStruct specified.").ToString(), this);
        // we break exec links so this is the only error we get
        BreakAllNodeLinks();
        return;
    }

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UUnLuaBlueprintLibrary, GetRowFromLua);
	UK2Node_CallFunction* GetRowFromLuaFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	GetRowFromLuaFunction->FunctionReference.SetExternalMember(FunctionName, UUnLuaBlueprintLibrary::StaticClass());
	GetRowFromLuaFunction->AllocateDefaultPins();
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(GetRowFromLuaFunction->GetExecPin()));

	// Connect the input of our GetRowFromLua to the Input of our Function pin
    UEdGraphPin* TargetStructInPin = GetRowFromLuaFunction->FindPinChecked(TEXT("TargetStruct"));
	if(OriginalTargetStructInPin->LinkedTo.Num() > 0)
	{
		// Copy the connection
		CompilerContext.MovePinLinksToIntermediate(*OriginalTargetStructInPin, *TargetStructInPin);
	}
	else
	{
		// Copy literal
		TargetStructInPin->DefaultObject = OriginalTargetStructInPin->DefaultObject;
	}
	UEdGraphPin* LuaTableNameInPin = GetRowFromLuaFunction->FindPinChecked(TEXT("LuaTableName"));
	CompilerContext.MovePinLinksToIntermediate(*GetLuaTableNamePin(), *LuaTableNameInPin);

	UEdGraphPin* RowNameInPin = GetRowFromLuaFunction->FindPinChecked(TEXT("RowName"));
	CompilerContext.MovePinLinksToIntermediate(*GetRowNamePin(), *RowNameInPin);

	// Get some pins to work with
	UEdGraphPin* OriginalOutRowPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	UEdGraphPin* FunctionOutRowPin = GetRowFromLuaFunction->FindPinChecked(TEXT("OutRow"));
    UEdGraphPin* FunctionReturnPin = GetRowFromLuaFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* FunctionThenPin = GetRowFromLuaFunction->GetThenPin();
        
    // Set the type of the OutRow pin on this expanded mode to match original
    FunctionOutRowPin->PinType = OriginalOutRowPin->PinType;
	FunctionOutRowPin->PinType.PinSubCategoryObject = OriginalOutRowPin->PinType.PinSubCategoryObject;
        
    //BRANCH NODE
    UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
    BranchNode->AllocateDefaultPins();
    // Hook up inputs to branch
    FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
    FunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());
        
    // Hook up outputs
    CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
    CompilerContext.MovePinLinksToIntermediate(*GetRowNotFoundPin(), *(BranchNode->GetElsePin()));
    CompilerContext.MovePinLinksToIntermediate(*OriginalOutRowPin, *FunctionOutRowPin);

	BreakAllNodeLinks();
}

FSlateIcon UK2Node_GetRowFromLua::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetRowFromLua::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPinType();
}

void UK2Node_GetRowFromLua::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	const UEdGraphPin* TargetStructPin = GetTargetStructPin();
	const UEdGraphPin* RowNamePin = GetRowNamePin();
	if (!TargetStructPin || !RowNamePin)
	{
		MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
		return;
	}

	if (TargetStructPin->LinkedTo.Num() == 0)
	{
		const UScriptStruct* TargetStruct = Cast<UScriptStruct>(TargetStructPin->DefaultObject);
		if (!TargetStruct)
		{
			MessageLog.Error(*LOCTEXT("NoTargetStruct", "No TargetStruct in @@").ToString(), this);
			return;
		}

		//if (!RowNamePin->LinkedTo.Num())
		//{
		//	const FName CurrentName = FName(*RowNamePin->GetDefaultAsString());
		//	if (!TargetStruct->GetRowNames().Contains(CurrentName))
		//	{
		//		const FString Msg = FText::Format(
		//			LOCTEXT("WrongRowNameFmt", "'{0}' row name is not stored in '{1}'. @@"),
		//			FText::FromString(CurrentName.ToString()),
		//			FText::FromString(GetFullNameSafe(TargetStruct))
		//		).ToString();
		//		MessageLog.Error(*Msg, this);
		//		return;
		//	}
		//}
	}	
}

void UK2Node_GetRowFromLua::PreloadRequiredAssets()
{
	if (UEdGraphPin* TargetStructPin = GetTargetStructPin())
	{
		if (UScriptStruct* TargetStruct = Cast<UScriptStruct>(TargetStructPin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetRowFromLuaStructType)
			PreloadObject(TargetStruct);
		}
	}
	return Super::PreloadRequiredAssets();
}

void UK2Node_GetRowFromLua::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetResultPin())
	{
		UEdGraphPin* StructPin = GetTargetStructPin();
		// this connection would only change the output type if the table pin is undefined
		const bool bIsTypeAuthority = (StructPin->LinkedTo.Num() > 0 || StructPin->DefaultObject == nullptr);
		if (bIsTypeAuthority)
		{
			RefreshOutputPinType();
		}		
	}
	else if (Pin == GetTargetStructPin())
	{
		const bool bConnectionAdded = Pin->LinkedTo.Num() > 0;
		if (bConnectionAdded)
		{
			// if a connection was made, then we may need to rid ourselves of the row dropdown
			RefreshRowNameOptions();
			// if the output connection was previously, incompatible, it now becomes the authority on this node's output type
			RefreshOutputPinType();
		}
	}
}

#undef LOCTEXT_NAMESPACE
