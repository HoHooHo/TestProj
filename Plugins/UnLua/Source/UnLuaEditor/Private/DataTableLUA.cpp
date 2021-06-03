// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataTableLUA.h"
#include "UObject/UnrealType.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetStringLibrary.h"
#include "Internationalization/Text.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "UObject/TextProperty.h"

FDataTableExporterLUA::FDataTableExporterLUA(const EDataTableExportFlags InDTExportFlags)
	: DTExportFlags(InDTExportFlags)
{
}

FDataTableExporterLUA::~FDataTableExporterLUA()
{
}

bool FDataTableExporterLUA::WriteTable(const UDataTable& InDataTable, const TArray<FString>& ExportFields, const FString& ExportFilename, bool bClient)
{
	if (!InDataTable.RowStruct)
	{
		return false;
	}

	// Write the header
	FString KeyString = TEXT("local keys = {\"ID\","), LangString = TEXT("local lang = {");
	for (TFieldIterator<FProperty> It(InDataTable.RowStruct); It; ++It)
	{
		FProperty* BaseProp = *It;
		check(BaseProp);
		FString PropertyName = BaseProp->GetAuthoredName();
		if (ExportFields.Contains(PropertyName))
		{
			KeyString += TEXT("\"") + PropertyName + TEXT("\",");
			if (BaseProp->IsA<FTextProperty>())
			{
				LangString += TEXT("\"") + PropertyName + TEXT("\",");
			}
		}
	}
	ExportedText += KeyString + TEXT("}\n");
	ExportedText += LangString + TEXT("}\n\n");

	if (bClient)
	{
		ExportedText += TEXT("local K = table.arrayToMap(keys, \"INDEX\")\n");
		ExportedText += TEXT("local L = table.arrayToMap(lang, \"INDEX\")\n");
		ExportedText += TEXT("local S = keys\n");
		ExportedText += TEXT("local C = CfgNodeData\n\n");
	}

	FString BaseFilename = FPaths::GetBaseFilename(ExportFilename);
	FString LuaTableName = TEXT("Origin") + BaseFilename;
	ExportedText += TEXT("local ") + LuaTableName + TEXT(" = \n{\n");

	// Write each row
	const TMap<FName, uint8*>& RowMap = InDataTable.GetRowMap();
	for (const auto& RowMapPair : RowMap)
	{
		FString RowName = RowMapPair.Key.ToString();
		if (bClient)
		{
			ExportedText += TEXT("\t[\'") + RowName + TEXT("\'] = C(K,S,L,{\"") + RowName + TEXT("\",");
			WriteRow(InDataTable.RowStruct, RowMapPair.Value, ExportFields);
			ExportedText += TEXT("}),\n");
		}
		else
		{
			ExportedText += TEXT("\t{\"") + RowName + TEXT("\",");
			WriteRow(InDataTable.RowStruct, RowMapPair.Value, ExportFields);
			ExportedText += TEXT("},\n");
		}
	}

	if (bClient)
	{
		ExportedText += TEXT("}\n\n");
	}
	else
	{
		ExportedText += TEXT("}\n\nlocal DATA = {}\n");
		ExportedText += TEXT("for i,v in pairs(") + LuaTableName + TEXT(") do\n");
		ExportedText += TEXT("\tlocal tmp = {}\n");
		ExportedText += TEXT("\tfor j,k in ipairs(keys) do\n");
		ExportedText += TEXT("\t\ttmp[k] = v[j]\n");
		ExportedText += TEXT("\tend\n");
		ExportedText += TEXT("\tDATA[v[1]] = tmp\n");
		ExportedText += TEXT("end\n");
		ExportedText += LuaTableName + TEXT(" = nil\n\n");
	}

	ExportedText += TEXT("_G.") + BaseFilename + TEXT("_Len = ") + UKismetStringLibrary::Conv_IntToString(RowMap.Num());
	ExportedText += TEXT("\n_G.") + BaseFilename + TEXT(" = ReadOnly.readOnly(") + (bClient ? LuaTableName : TEXT("DATA")) + TEXT(")\n");

	// Save lua file
	return FFileHelper::SaveStringToFile(ExportedText, *ExportFilename, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

void FDataTableExporterLUA::WriteObjectStart()
{
	ExportedText += TEXT("{");
}

void FDataTableExporterLUA::WriteObjectEnd()
{
	ExportedText += TEXT("},");
}

void FDataTableExporterLUA::WritePropertyValue(const FString& Value)
{
	ExportedText += Value + TEXT(",");
}

bool FDataTableExporterLUA::WriteRow(const UScriptStruct* InRowStruct, const void* InRowData, const TArray<FString>& ExportFields)
{
	for (TFieldIterator<FProperty> It(InRowStruct); It; ++It)
	{
		FProperty* BaseProp = *It;
		if (ExportFields.Contains(BaseProp->GetAuthoredName()))
		{
			if (BaseProp->ArrayDim == 1)
			{
				const void* Data = BaseProp->ContainerPtrToValuePtr<void>(InRowData, 0);
				WriteStructEntry(InRowData, BaseProp, Data, false);
			}
		}
	}

	return true;
}

bool FDataTableExporterLUA::WriteStruct(const UScriptStruct* InStruct, const void* InStructData, bool bWriteKey)
{
	for (TFieldIterator<FProperty> It(InStruct); It; ++It)
	{
		FProperty* BaseProp = *It;
		check(BaseProp);

		if (BaseProp->ArrayDim == 1)
		{
			const void* Data = BaseProp->ContainerPtrToValuePtr<void>(InStructData, 0);
			WriteStructEntry(InStructData, BaseProp, Data, bWriteKey);
		}
		else
		{
			WriteObjectStart();

			for (int32 ArrayEntryIndex = 0; ArrayEntryIndex < BaseProp->ArrayDim; ++ArrayEntryIndex)
			{
				const void* Data = BaseProp->ContainerPtrToValuePtr<void>(InStructData, ArrayEntryIndex);
				WriteContainerEntry(BaseProp, Data);
			}

			WriteObjectEnd();
		}
	}

	return true;
}

bool FDataTableExporterLUA::WriteStructEntry(const void* InRowData, FProperty* InProperty, const void* InPropertyData, bool bWriteKey)
{
	if (bWriteKey)
	{
		ExportedText += DataTableUtils::GetPropertyExportName(InProperty, DTExportFlags) + TEXT("=");
	}

	if (const FEnumProperty* EnumProp = CastField<const FEnumProperty>(InProperty))
	{
		FString PropertyValue = EnumProp->GetUnderlyingProperty()->GetNumericPropertyValueToString(InPropertyData);
		WritePropertyValue(PropertyValue);
	}
	else if (const FNumericProperty* NumProp = CastField<const FNumericProperty>(InProperty))
	{
		FString PropertyValue = NumProp->GetNumericPropertyValueToString(InPropertyData);
		WritePropertyValue(PropertyValue);
	}
	else if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(InProperty))
	{
		const bool PropertyValue = BoolProp->GetPropertyValue(InPropertyData);
		WritePropertyValue(UKismetStringLibrary::Conv_BoolToString(PropertyValue));
	}
	else if (const FArrayProperty* ArrayProp = CastField<const FArrayProperty>(InProperty))
	{
		WriteObjectStart();

		FScriptArrayHelper ArrayHelper(ArrayProp, InPropertyData);
		for (int32 ArrayEntryIndex = 0; ArrayEntryIndex < ArrayHelper.Num(); ++ArrayEntryIndex)
		{
			const uint8* ArrayEntryData = ArrayHelper.GetRawPtr(ArrayEntryIndex);
			WriteContainerEntry(ArrayProp->Inner, ArrayEntryData);
		}

		WriteObjectEnd();
	}
	else if (const FSetProperty* SetProp = CastField<const FSetProperty>(InProperty))
	{
		WriteObjectStart();

		FScriptSetHelper SetHelper(SetProp, InPropertyData);
		for (int32 SetSparseIndex = 0; SetSparseIndex < SetHelper.GetMaxIndex(); ++SetSparseIndex)
		{
			if (SetHelper.IsValidIndex(SetSparseIndex))
			{
				const uint8* SetEntryData = SetHelper.GetElementPtr(SetSparseIndex);
				WriteContainerEntry(SetHelper.GetElementProperty(), SetEntryData);
			}
		}

		WriteObjectEnd();
	}
	else if (const FMapProperty* MapProp = CastField<const FMapProperty>(InProperty))
	{
		WriteObjectStart();

		FScriptMapHelper MapHelper(MapProp, InPropertyData);
		for (int32 MapSparseIndex = 0; MapSparseIndex < MapHelper.GetMaxIndex(); ++MapSparseIndex)
		{
			if (MapHelper.IsValidIndex(MapSparseIndex))
			{
				const uint8* MapKeyData = MapHelper.GetKeyPtr(MapSparseIndex);
				const uint8* MapValueData = MapHelper.GetValuePtr(MapSparseIndex);

				// JSON object keys must always be strings
				FString Key = DataTableUtils::GetPropertyValueAsStringDirect(MapHelper.GetKeyProperty(), (uint8*)MapKeyData, DTExportFlags);
				Key = Key.Replace(TEXT("("), TEXT("{"));
				Key = Key.Replace(TEXT(")"), TEXT("}"));
				ExportedText += TEXT("[") + Key + TEXT("]=");
				WriteContainerEntry(MapHelper.GetValueProperty(), MapValueData);
			}
		}

		WriteObjectEnd();
	}
	else if (const FStructProperty* StructProp = CastField<const FStructProperty>(InProperty))
	{
		WriteObjectStart();
		WriteStruct(StructProp->Struct, InPropertyData, true);
		WriteObjectEnd();
	}
	else if (const FTextProperty* TextProp = CastField<const FTextProperty>(InProperty))
	{
		FName TableId;
		FString Key;
		FText Text = TextProp->GetPropertyValue(InPropertyData);
		FTextInspector::GetTableIdAndKey(Text, TableId, Key);
		WritePropertyValue(TEXT("\"") + Key + TEXT("\""));
	}
	else
	{
		FString PropertyValue = DataTableUtils::GetPropertyValueAsString(InProperty, (uint8*)InRowData, DTExportFlags);
		WritePropertyValue(TEXT("\"") + PropertyValue + TEXT("\""));
	}

	return true;
}

bool FDataTableExporterLUA::WriteContainerEntry(const FProperty* InProperty, const void* InPropertyData)
{
	if (const FEnumProperty* EnumProp = CastField<const FEnumProperty>(InProperty))
	{
		FString PropertyValue = EnumProp->GetUnderlyingProperty()->GetNumericPropertyValueToString(InPropertyData);
		WritePropertyValue(PropertyValue);
	}
	else if (const FNumericProperty* NumProp = CastField<const FNumericProperty>(InProperty))
	{
		FString PropertyValue = NumProp->GetNumericPropertyValueToString(InPropertyData);
		WritePropertyValue(PropertyValue);
	}
	else if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(InProperty))
	{
		const bool PropertyValue = BoolProp->GetPropertyValue(InPropertyData);
		WritePropertyValue(UKismetStringLibrary::Conv_BoolToString(PropertyValue));
	}
	else if (const FStructProperty* StructProp = CastField<const FStructProperty>(InProperty))
	{
		WriteObjectStart();
		WriteStruct(StructProp->Struct, InPropertyData, true);
		WriteObjectEnd();
	}
	else if (const FArrayProperty* ArrayProp = CastField<const FArrayProperty>(InProperty))
	{
		// Cannot nest arrays
		return false;
	}
	else if (const FSetProperty* SetProp = CastField<const FSetProperty>(InProperty))
	{
		// Cannot nest sets
		return false;
	}
	else if (const FMapProperty* MapProp = CastField<const FMapProperty>(InProperty))
	{
		// Cannot nest maps
		return false;
	}
	else
	{
		const FString PropertyValue = DataTableUtils::GetPropertyValueAsStringDirect(InProperty, (uint8*)InPropertyData, DTExportFlags);
		WritePropertyValue(TEXT("\"") + PropertyValue + TEXT("\""));
	}

	return true;
}