// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnLuaBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNLUA_API UUnLuaBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static int GetLuaFiles();

	UFUNCTION(BlueprintCallable)
	static FString GetLuaFileName(int i);

	UFUNCTION(BlueprintCallable)
	static FString GetLuaModName(int i);

	UFUNCTION(BlueprintCallable)
	static FString GetLuaFilePath(int i);

	UFUNCTION(BlueprintCallable, Category = "UTL")
	static FString UObjectToLuaString(UObject* Object, bool DefaultAsEmpty = false);

	UFUNCTION(BlueprintCallable, Category = "UTL")
	static FString UObjectToLuaStringWithIgnore(UObject* Object, FString ignore, bool DefaultAsEmpty = false);

	UFUNCTION(BlueprintCallable, Category = "UTL")
	static UObject* LuaStringToUObject(UClass* className, FString nameAndValue);

	UFUNCTION(BlueprintCallable, Category = "UTL")
	static UObject* LuaStringToSetUObject(UObject* Object, FString nameAndValue);

	UFUNCTION(BlueprintCallable)
	static bool ScriptExists(const FString& LuaFilePath);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutRow", BlueprintInternalUseOnly = "true"))
	static bool GetRowFromLua(FName LuaTableName, FName RowName, const UScriptStruct* TargetStruct, FTableRowBase& OutRow);

	static bool Generic_GetRowFromLua(FName LuaTableName, FName RowName, UScriptStruct* TargetStruct, void* OutRowPtr);

	DECLARE_FUNCTION(execGetRowFromLua)
	{
		P_GET_PROPERTY(FNameProperty, LuaTableName);
		P_GET_PROPERTY(FNameProperty, RowName);
		P_GET_OBJECT(UScriptStruct, TargetStruct);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;
		bool bSuccess = false;

		if (StructProp && OutRowPtr)
		{
			UScriptStruct* OutputType = StructProp->Struct;

			const bool bCompatible = (OutputType == TargetStruct) ||
				(OutputType->IsChildOf(TargetStruct) && FStructUtils::TheSameLayout(OutputType, TargetStruct));
			if (bCompatible)
			{
				P_NATIVE_BEGIN;
				bSuccess = Generic_GetRowFromLua(LuaTableName, RowName, TargetStruct, OutRowPtr);
				P_NATIVE_END;
			}
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("GetRowFromLua", "MissingOutputProperty", "Failed to resolve the output parameter for GetRowFromLua.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
		*(bool*)RESULT_PARAM = bSuccess;
	}

private:
	static FString ToCompressFloatStr(float F, bool DefaultAsEmpty = false);
	static FString ToCompressDoubleStr(double F, bool DefaultAsEmpty = false);
	static FString ToObjectStr(UObject* Object, bool DefaultAsEmpty = false);

	static FString CheckPropertyType(FProperty* Property, void* Object, bool DefaultAsEmpty = false);
	static FString GetPropertyValueLuaStr(FProperty* Property, void* Object, bool DefaultAsEmpty = false);

	//检查Object的Uproperty的类型并把value字符串转换成对应值并赋值
	static void CheckSetType(FProperty* Property, void* Object, FString value);

	//Object的UObjectProperty赋值用，返回一个UObject*
	static UObject* SetObj(UObject* Object, FString nameAndValue);

	static TArray<FString> LuaModNames;
	static TArray<FString> LuaFileNames;
	static TArray<FString> LuaFilePaths;
};
