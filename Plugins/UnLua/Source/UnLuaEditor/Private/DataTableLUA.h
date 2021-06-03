// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UDataTable;
enum class EDataTableExportFlags : uint8;

class FDataTableExporterLUA
{
public:
	FDataTableExporterLUA(const EDataTableExportFlags InDTExportFlags);

	~FDataTableExporterLUA();

	/** Writes the data table out as an array of objects */
	bool WriteTable(const UDataTable& InDataTable, const TArray<FString>& ExportFields, const FString& ExportFilename, bool bClient);

private:
	void WriteObjectStart();
	void WriteObjectEnd();
	void WritePropertyValue(const FString& Value);
	bool WriteRow(const UScriptStruct* InRowStruct, const void* InRowData, const TArray<FString>& ExportFields);
	bool WriteStruct(const UScriptStruct* InStruct, const void* InStructData, bool bWriteKey);
	bool WriteStructEntry(const void* InRowData, FProperty* InProperty, const void* InPropertyData, bool bWriteKey);
	bool WriteContainerEntry(const FProperty* InProperty, const void* InPropertyData);

	EDataTableExportFlags DTExportFlags;
	FString ExportedText;
};
