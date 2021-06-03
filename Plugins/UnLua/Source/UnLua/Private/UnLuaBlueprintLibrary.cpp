// Fill out your copyright notice in the Description page of Project Settings.


#include "UnLuaBlueprintLibrary.h"
#include "Engine/UserDefinedStruct.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "UnLua.h"
#include "UnLuaPrivate.h"

TArray<FString> UUnLuaBlueprintLibrary::LuaModNames;
TArray<FString> UUnLuaBlueprintLibrary::LuaFileNames;
TArray<FString> UUnLuaBlueprintLibrary::LuaFilePaths;

int UUnLuaBlueprintLibrary::GetLuaFiles()
{
	if (LuaFileNames.Num() > 0)
	{
		LuaModNames.Empty();
		LuaFileNames.Empty();
		LuaFilePaths.Empty();
	}

	TArray<FString> FoundFiles;
	int32 LuaPathLen = GLuaSrcFullPath.Len();
	FPlatformFileManager::Get().GetPlatformFile().FindFilesRecursively(FoundFiles, *GLuaSrcFullPath, nullptr);
	for (const FString& File : FoundFiles)
	{
		if (FPaths::GetExtension(File) == TEXT("lua"))
		{
			LuaModNames.Add(File.Mid(LuaPathLen, File.Len() - LuaPathLen - 4));
			LuaFileNames.Add(FPaths::GetBaseFilename(File));
			LuaFilePaths.Add(File);
		}
	}
	return LuaFileNames.Num();
}

FString UUnLuaBlueprintLibrary::GetLuaFileName(int i)
{
	return LuaFileNames[i];
}

FString UUnLuaBlueprintLibrary::GetLuaModName(int i)
{
	return LuaModNames[i];
}

FString UUnLuaBlueprintLibrary::GetLuaFilePath(int i)
{
	return LuaFilePaths[i];
}

//获取FProperty的模板
template <typename ValueType>
ValueType GetPropertyValue(FProperty* Property, void* Object)
{
	return *Property->ContainerPtrToValuePtr<ValueType>(Object);
}

//设置FProperty的模板
template <typename ValueType>
bool SetPropertyValue(FProperty* Property, void* Object, ValueType Value)
{
	// Get the property addresses for the object
	ValueType* SourceAddr = Property->ContainerPtrToValuePtr<ValueType>(Object);

	if (SourceAddr == NULL)
	{
		return false;
	}

	// Set the value on the destination object.
	*SourceAddr = Value;
	return true;
}

FString UUnLuaBlueprintLibrary::ToCompressFloatStr(float F, bool DefaultAsEmpty)
{
	float Res = (int)(F * 100.0f + 0.5f) / 100.0f;
	if (DefaultAsEmpty && FMath::IsNearlyZero(Res))
	{
		return FString();
	}
	else
	{
		return FString::SanitizeFloat(Res);
	}
}

FString UUnLuaBlueprintLibrary::ToCompressDoubleStr(double F, bool DefaultAsEmpty)
{
	double Res = (int)(F * 100.0 + 0.5f) / 100.0;
	if (DefaultAsEmpty && FMath::IsNearlyZero(Res))
	{
		return FString();
	}
	else
	{
		return FString::SanitizeFloat(Res);
	}
}

FString UUnLuaBlueprintLibrary::ToObjectStr(UObject* Object, bool DefaultAsEmpty)
{
	int PropertyNum = 0;
	FString TotalALLNames;
	for (TFieldIterator<FProperty> PropertyIt(Object->GetClass()); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		if (Property->HasAnyPropertyFlags(CPF_Deprecated | CPF_Transient))
		{
			continue;
		}

		FString KVStr = CheckPropertyType(Property, Object, DefaultAsEmpty);
		if (!KVStr.IsEmpty())
		{
			if (PropertyNum++ > 0)
			{
				TotalALLNames.Append(",");
			}
			TotalALLNames.Append(KVStr);
		}
	}

	if (DefaultAsEmpty && TotalALLNames.IsEmpty())
	{
		return FString();
	}
	else
	{
		return FString::Printf(TEXT("{%s}"), *TotalALLNames);
	}
}

FString UUnLuaBlueprintLibrary::CheckPropertyType(FProperty* Property, void* Object, bool DefaultAsEmpty)
{
	const FString& PropertyStr = GetPropertyValueLuaStr(Property, Object, DefaultAsEmpty);
	if (PropertyStr.IsEmpty() && DefaultAsEmpty)
	{
		return FString();
	}
	else
	{
		return FString::Printf(TEXT("%s=%s"), *Property->GetName(), *PropertyStr);
	}
}

FString UUnLuaBlueprintLibrary::GetPropertyValueLuaStr(FProperty* Property, void* Object, bool DefaultAsEmpty)
{
	if (FInt8Property* I8Property = CastField<FInt8Property>(Property))
	{
		int8 Value = I8Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::FromInt(Value);
	}
	else if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
	{
		uint8 Value = ByteProperty->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::FromInt(Value);
	}
	else if (FInt16Property* I16Property = CastField<FInt16Property>(Property))
	{
		int16 Value = I16Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::FromInt(Value);
	}
	else if (FUInt16Property* UI16Property = CastField<FUInt16Property>(Property))
	{
		uint16 Value = UI16Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::FromInt(Value);
	}
	else if (FIntProperty* I32Property = CastField<FIntProperty>(Property))
	{
		int32 Value = I32Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::FromInt(Value);
	}
	else if (FUInt32Property* UI32Property = CastField<FUInt32Property>(Property))
	{
		uint32 Value = UI32Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::Printf(TEXT("%d"), Value);
	}
	else if (FInt64Property* I64Property = CastField<FInt64Property>(Property))
	{
		int64 Value = I64Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::Printf(TEXT("%lld"), Value);
	}
	else if (FUInt64Property* UI64Property = CastField<FUInt64Property>(Property))
	{
		uint64 Value = UI64Property->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value == 0 ? FString() : FString::Printf(TEXT("%lld"), Value);
	}
	if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		bool BoolValue = BoolProperty->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && !BoolValue ? FString() : FString(BoolValue ? "true" : "false");
	}
	else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
	{
		float Value = FloatProperty->GetPropertyValue_InContainer(Object);
		return ToCompressFloatStr(Value, DefaultAsEmpty);
	}
	else if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
	{
		double Value = DoubleProperty->GetPropertyValue_InContainer(Object);
		return ToCompressDoubleStr(Value, DefaultAsEmpty);
	}
	else if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
	{
		FString Value = StrProperty->GetPropertyValue_InContainer(Object);
		return DefaultAsEmpty && Value.IsEmpty() ? FString() : Value;
	}
	else if (FNameProperty* NameProperty = CastField<FNameProperty>(Property))
	{
		return NameProperty->GetPropertyValue_InContainer(Object).ToString();
	}
	else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
	{
		FString ArrayAllNames = "{";
		int NilNum = 0, ArrNum = 0;
		FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Object));
		for (int i = 0; i < ArrayHelper.Num(); i++)
		{
			void* ValuePtr = ArrayHelper.GetRawPtr(i);
			FString InnerStr = GetPropertyValueLuaStr(ArrayProperty->Inner, ValuePtr, DefaultAsEmpty);
			if (DefaultAsEmpty && InnerStr.IsEmpty())
			{
				NilNum++;
			}
			else
			{
				if (ArrNum > 0)
				{
					ArrayAllNames.Append(",");
				}

				ArrNum += NilNum;
				for (int j = 0; j < NilNum; j++)
				{
					ArrayAllNames.Append("nil,");
				}
				NilNum = 0;

				ArrayAllNames.Append(InnerStr.IsEmpty() ? TEXT("nil") : *InnerStr);
				ArrNum++;
			}
		}
		if (DefaultAsEmpty && ArrNum == 0)
		{
			return FString();
		}
		else
		{
			ArrayAllNames.Append("}");
			return ArrayAllNames;
		}
	}
	else if (FMapProperty* MapProperty = CastField<FMapProperty>(Property))
	{
		FString KeyName;
		FString ValueName;
		int MapNum = 0;
		FString MapStr = "{";
		FScriptMapHelper MapHelper(MapProperty, MapProperty->ContainerPtrToValuePtr<void>(Object));
		for (int i = 0; i < MapHelper.Num(); i++)
		{
			KeyName = GetPropertyValueLuaStr(MapProperty->KeyProp, MapHelper.GetKeyPtr(i), DefaultAsEmpty);
			if (KeyName.IsEmpty())
			{
				continue;
			}
			ValueName = GetPropertyValueLuaStr(MapProperty->ValueProp, MapHelper.GetValuePtr(i), DefaultAsEmpty);
			if (ValueName.IsEmpty())
			{
				continue;
			}
			if (MapNum > 0)
			{
				MapStr.Append(",");
			}
			MapStr.Append(FString::Printf(TEXT("[%s]=%s"), *KeyName, *ValueName));
			MapNum++;
		}
		if (DefaultAsEmpty && MapNum == 0)
		{
			return FString();
		}
		else
		{
			MapStr.Append("}");
			return MapStr;
		}
	}
	else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		int32 PropertyNum = 0;
		FString StructStr = "{";
		void* StructAddress = StructProperty->ContainerPtrToValuePtr<void>(Object);
		for (TFieldIterator<FProperty> It(StructProperty->Struct); It; ++It)
		{
			FProperty* ChildProperty = *It;
			if (ChildProperty->HasAnyPropertyFlags(CPF_Deprecated | CPF_Transient))
			{
				continue;
			}
			if (PropertyNum++ > 0)
			{
				StructStr += TEXT(",");
			}
			StructStr += CheckPropertyType(ChildProperty, StructAddress, DefaultAsEmpty);
		}
		StructStr += "}";
		return StructStr;
	}
	else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		UObject* tempUObject = GetPropertyValue<UObject*>(ObjectProperty, Object);

		if (!tempUObject)
		{
			if (DefaultAsEmpty)
			{
				return FString();
			}
			else
			{
				return FString("{}");
			}
		}
		else
		{
			return ToObjectStr(tempUObject, DefaultAsEmpty);
		}
	}
	else if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
	{
		FScriptSetHelper SetHelper(SetProperty, SetProperty->ContainerPtrToValuePtr<void>(Object));
		int SetNum = 0;
		FString SetAllNames = "{";
		for (int i = 0; i < SetHelper.Num(); i++)
		{
			void* ValuePtr = SetHelper.GetElementPtr(i);
			const FString& Key = GetPropertyValueLuaStr(SetProperty->ElementProp, ValuePtr, DefaultAsEmpty);
			if (Key.IsEmpty())
			{
			}
			else
			{
				if (SetNum > 0)
				{
					SetAllNames.Append(",");
				}
				SetNum++;
				SetAllNames.Append(FString::Printf(TEXT("[%s]=true"), *Key));
			}
		}

		if (DefaultAsEmpty && SetNum == 0)
		{
			return FString();
		}
		else
		{
			SetAllNames.Append("}");
			return SetAllNames;
		}
	}
	else if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		uint8 tempint8 = GetPropertyValue<uint8>(Property, Object);
		if (DefaultAsEmpty && tempint8 == 0)
		{
			return FString();
		}
		else
		{
			return FString::FromInt(tempint8);
		}
	}

	UE_LOG(LogUnLua, Warning, TEXT("Unrecognized Property: %s"), *Property->GetName());
	if (DefaultAsEmpty)
	{
		return FString();
	}
	else
	{
		return FString("nil");
	}
}

void UUnLuaBlueprintLibrary::CheckSetType(FProperty* Property, void* Object, FString value)
{
	if (CastField<FBoolProperty>(Property))
	{
		bool tempBool = value.ToBool();
		SetPropertyValue(Property, Object, tempBool);
	}
	else if (CastField<FIntProperty>(Property))
	{
		int32 tempInt = FCString::Atoi(*value);
		SetPropertyValue(Property, Object, tempInt);
	}
	else if (CastField<FFloatProperty>(Property))
	{
		float tempFloat = FCString::Atof(*value);
		SetPropertyValue(Property, Object, tempFloat);
	}
	else if (CastField<FStrProperty>(Property))
	{
		value.RemoveAt(0, 1);
		value.RemoveAt(value.Len() - 1, 1);
		SetPropertyValue(Property, Object, value);
	}
	else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
	{
		value.RemoveAt(0, 1);
		value.RemoveAt(value.Len() - 1, 1);

		FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Object));

		int numOfArray = 0;//设置的目标数组的元素个数
		FString temArrayVal;

		int flags = 0;
		for (int i = 0; i < value.Len(); i++)
		{
			if (value[i] == '{')
			{
				flags++;
			}
			if (value[i] == '}')
			{
				flags--;
			}
			if (value[i] == ',' && flags == 0)
			{
				numOfArray++;
			}
		}
		flags = 0;
		//目标数组元素个数大于0 需要重新设置数组大小
		if (numOfArray >= 0)
		{
			ArrayHelper.Resize(numOfArray);
		}
		int count = 0;//
		//依次设置数组的每个元素
		for (int i = 0; i < ArrayHelper.Num(); i++)
		{
			//处理value，取出对应索引的元素值
			for (; count < value.Len(); count++)
			{
				if (value[count] == '{')
				{
					flags++;
				}
				if (value[count] == '}')
				{
					flags--;
				}
				if (value[count] != '{' && value[count] != '}')
				{
					//遇到逗号跳过
					if (value[count] == ',' && flags == 0)
					{
						count++;
						break;
					}
					else if (value[count] == ',' && flags != 0)
					{
						temArrayVal += value[count];
						continue;
					}
					temArrayVal += value[count];
				}
			}
			if (FObjectProperty* tempObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner))
			{
				void* ValuePtr = ArrayHelper.GetRawPtr(i);
				//CheckSetType(ArrayProperty->Inner, ValuePtr, temArrayVal);
				UClass* tempClass = tempObjectProperty->PropertyClass;
				UObject* object = NewObject<UObject>(tempClass, tempClass);

				if (temArrayVal[0] != '{' && temArrayVal[temArrayVal.Len() - 1] != '}')
				{
					temArrayVal = "{" + temArrayVal + "}";
				}
				object = SetObj(object, temArrayVal);

				FProperty* tempProperty = ArrayProperty->Inner;

				/*UObject* SourceAddr = tempProperty->ContainerPtrToValuePtr<UObject>(ValuePtr);
				SourceAddr = object;*/
				*tempProperty->ContainerPtrToValuePtr<UObject*>(ValuePtr) = object;
				temArrayVal.Empty();
			}
			else
			{
				void* ValuePtr = ArrayHelper.GetRawPtr(i);
				CheckSetType(ArrayProperty->Inner, ValuePtr, temArrayVal);
				temArrayVal.Empty();
			}
		}
	}
	else if (FMapProperty* MapProperty = CastField<FMapProperty>(Property))
	{
		//----进一步处理map的字符串
		TArray<FString> mapKey;//存放key值
		TArray<FString> mapValue;//存放value值
		FString mapK;//临时存放mapkey值
		FString mapV;//临时存放mapvalue值

		//去掉前后的大括号
		value.RemoveAt(0, 1);
		value.RemoveAt(value.Len() - 1, 1);

		int flagForLeft = 0;//标记左中括号

		//拆分key和value
		for (int i = 0; i < value.Len(); i++)
		{
			if (value[i] == '[')
			{
				flagForLeft++;
				continue;
			}
			if (value[i] == ']')
			{
				flagForLeft--;
				continue;
			}
			if (flagForLeft && value[i] != ',')
			{
				mapK = mapK + value[i];
			}
			else if (value[i] != '=' && value[i] != ',')
			{
				mapV = mapV + value[i];
			}
			if (value[i] == ',')
			{
				mapKey.Add(mapK);
				mapValue.Add(mapV);
				mapK.Empty();
				mapV.Empty();
			}
		}

		FScriptMapHelper MapHelper(MapProperty, MapProperty->ContainerPtrToValuePtr<void>(Object));
		MapHelper.EmptyValues();//清空原map
		//根据目标Map设置新Map的大小
		for (auto& i : mapKey)
		{
			MapHelper.AddDefaultValue_Invalid_NeedsRehash();
		}
		//设置key和value的值
		for (int i = 0; i < mapKey.Num(); i++)
		{
			CheckSetType(MapProperty->KeyProp, MapHelper.GetKeyPtr(i), mapKey[i]);
			CheckSetType(MapProperty->ValueProp, MapHelper.GetKeyPtr(i), mapValue[i]);
		}
	}
	else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if (StructProperty->Struct == TBaseStructure<FVector>::Get())
		{
			FString tempFv;
			TArray<float> Fv;
			for (int i = 0; i < value.Len(); i++)
			{
				if ((value[i] >= '0' && value[i] <= '9') || value[i] == '.')
				{
					tempFv = tempFv + value[i];
				}
				else if (value[i] == ',')
				{
					Fv.Add(FCString::Atof(*tempFv));
					tempFv.Empty();
					continue;
				}
			}
			SetPropertyValue(Property, Object, FVector(Fv[0], Fv[1], Fv[2]));
		}
		else if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
		{
			FString tempFr;
			TArray<float> Fr;
			for (int i = 0; i < value.Len(); i++)
			{
				if ((value[i] >= '0' && value[i] <= '9') || value[i] == '.')
				{
					tempFr = tempFr + value[i];
				}
				else if (value[i] == ',')
				{
					Fr.Add(FCString::Atof(*tempFr));
					tempFr.Empty();
					continue;
				}
			}
			SetPropertyValue(Property, Object, FRotator(Fr[0], Fr[1], Fr[2]));
		}
		else if (StructProperty->Struct == TBaseStructure<FSoftObjectPath>::Get())
		{
			value.RemoveAt(0, 1);
			value.RemoveAt(value.Len() - 1, 1);
			SetPropertyValue(Property, Object, FSoftObjectPath(value));
		}
		else if (StructProperty->Struct == TBaseStructure<FSoftClassPath>::Get())
		{
			value.RemoveAt(0, 1);
			value.RemoveAt(value.Len() - 1, 1);
			SetPropertyValue(Property, Object, FSoftClassPath(value));
		}
	}
	else if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
	{
		//----进一步处理m字符串
		TArray<FString> setKey;//存放key值

		//去掉前后的大括号
		value.RemoveAt(0, 1);
		value.RemoveAt(value.Len() - 1, 1);

		FScriptSetHelper setHelper(SetProperty, SetProperty->ContainerPtrToValuePtr<void>(Object));

		//按照逗号拆分
		FString tempFv = "";
		for (int i = 0; i < value.Len(); i++)
		{
			if (value[i] != ',')
			{
				tempFv += value[i];
			}
			else if (value[i] == ',')
			{
				//去掉=true
				//tempFv.RemoveAt(0, 1);
				tempFv.RemoveAt(tempFv.Len() - 5, 5);

				setKey.Add(tempFv);
				tempFv.Empty();
				continue;
			}
		}

		setHelper.EmptyElements();

		for (int i = 0; i < setKey.Num(); i++)
		{
			setHelper.AddUninitializedValue();
			void* valuePtr = setHelper.GetElementPtr(i);
			CheckSetType(SetProperty->ElementProp, valuePtr, setKey[i]);
		}
	}
	else if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		UEnum* tempEnum = EnumProperty->GetEnum();
		int32 strint = FCString::Atoi(*value);
		FName name = tempEnum->GetNameByValue(strint);
		SetPropertyValue(Property, Object, strint);
	}
	else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		if (value[0] != '{' && value[value.Len() - 1] != '}')
		{
			value = "{" + value + "}";
		}
		UObject* object = GetPropertyValue<UObject*>(ObjectProperty, Object);
		if (object == NULL)
		{
			object = NewObject<UObject>(ObjectProperty->PropertyClass, ObjectProperty->PropertyClass);
		}
		SetObj(object, value);
	}
}

UObject* UUnLuaBlueprintLibrary::SetObj(UObject* Object, FString nameAndValue)
{
	nameAndValue.RemoveAt(0, 1);
	nameAndValue.RemoveAt(nameAndValue.Len() - 1, 1);
	FString name;
	FString value;
	TArray<FString> nameArray;
	TArray<FString> valueArray;
	TMap<FString, FString> NaVMap;
	int flags = 0;
	int leftBigPara = 0;
	int flagForApo = 0;

	//把字符串拆好 放进map里
	for (int i = 0; i < nameAndValue.Len(); i++)
	{
		if (nameAndValue[i] == '=')
		{
			flags++;
			if (flags > 1)
			{
				value += nameAndValue[i];
			}
		}

		if (nameAndValue[i] != '=')
		{
			if (nameAndValue[i] == '}')
			{
				leftBigPara--;
			}

			if (flags)//等号数量大于零
			{
				value += nameAndValue[i];
			}
			else
			{
				name += nameAndValue[i];
			}

			if (leftBigPara == 0)
			{
				if (nameAndValue[i] == '}')
				{
					i++;
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}

				if (flags == 1 && nameAndValue[i] == ',')
				{
					value.RemoveAt(value.Len() - 1, 1);
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}

				if (flags >= 1 && leftBigPara == 0 && nameAndValue[i] == ',' && nameAndValue[i - 1] == '\'')
				{
					value.RemoveAt(value.Len() - 1, 1);
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}
			}
			continue;
		}
		else
		{
			if (nameAndValue[i + 1] == '{')
			{
				leftBigPara++;
				if (nameAndValue[i] != '=')
				{
					value += nameAndValue[i];
				}
			}
		}
	}

	if (Object == NULL || Object->GetClass() == NULL)
	{
		return Object;
	}
	//开始set
	for (TFieldIterator<FProperty> PropertyIterator(Object->GetClass()); PropertyIterator; ++PropertyIterator)
	{
		FProperty* Property = *PropertyIterator;
		FString PropertyName = Property->GetName();

		for (int i = 0; i < NaVMap.Num(); i++)
		{
			if (PropertyName == nameArray[i])
			{
				CheckSetType(Property, Object, valueArray[i]);
				break;
			}
		}
	}

	return Object;
}

FString UUnLuaBlueprintLibrary::UObjectToLuaString(UObject* Object, bool DefaultAsEmpty)
{
	return FString::Printf(TEXT("return %s"), *ToObjectStr(Object, DefaultAsEmpty));
}

FString UUnLuaBlueprintLibrary::UObjectToLuaStringWithIgnore(UObject* Object, FString Ignore, bool DefaultAsEmpty)
{
	TArray<FString> ignoreArray;
	FString MatchStr;
	FString RestStr = Ignore;
	while (RestStr.Split(",", &MatchStr, &RestStr))
	{
		ignoreArray.Add(MatchStr);
	}
	ignoreArray.Add(RestStr);

	if (ignoreArray.Num() <= 0)
	{
		return UUnLuaBlueprintLibrary::UObjectToLuaString(Object, DefaultAsEmpty);
	}

	FString TotalALLNames;
	int ElNum = 0;

	//遍历所有被FProperty标记的成员
	for (TFieldIterator<FProperty> PropertyIterator(Object->GetClass()); PropertyIterator; ++PropertyIterator)
	{
		FProperty* Property = *PropertyIterator;
		FString PropertyName = Property->GetName();

		//判断需要忽略的属性名称
		if (ignoreArray.Contains(PropertyName))
		{
			continue;
		}

		//判断FProperty类型
		FString KVStr = CheckPropertyType(Property, Object, DefaultAsEmpty);
		if (!KVStr.IsEmpty())
		{
			if (ElNum > 0)
			{
				TotalALLNames.Append(",");
			}
			TotalALLNames.Append(KVStr);
			ElNum++;
		}
	}
	if (DefaultAsEmpty && TotalALLNames.IsEmpty())
	{
		return FString();
	}
	else
	{
		return FString::Printf(TEXT("return {%s}"), *TotalALLNames);
	}
}

UObject* UUnLuaBlueprintLibrary::LuaStringToUObject(UClass* className, FString nameAndValue)
{
	UObject* Object = NewObject<UObject>(className, className);
	Object = UUnLuaBlueprintLibrary::LuaStringToSetUObject(Object, nameAndValue);
	return Object;
}

UObject* UUnLuaBlueprintLibrary::LuaStringToSetUObject(UObject* Object, FString nameAndValue)
{
	//如果有return开头的话就去掉
	nameAndValue = nameAndValue.Replace(TEXT("return"), TEXT(""));

	//去掉前后最外层的大括号
	if (nameAndValue[0] == '{')
	{
		nameAndValue.RemoveAt(0, 1);
		nameAndValue.RemoveAt(nameAndValue.Len() - 1, 1);
	}

	FString name;//临时存放变量名
	FString value;//临时存放变量值
	TArray<FString> nameArray;//变量名的数组
	TArray<FString> valueArray;//变量值的数组
	TMap<FString, FString> NaVMap;
	int flags = 0;//标记等号
	int leftBigPara = 0;//标记{

	//把字符串拆好 放进两个数组里
	for (int i = 0; i < nameAndValue.Len(); i++)
	{
		if (nameAndValue[i] == '=')
		{
			flags++;
			if (flags > 1)
			{
				value += nameAndValue[i];
			}
		}
		if (nameAndValue[i] == '{' && nameAndValue[i - 1] == ',')
		{
			leftBigPara++;
		}

		if (nameAndValue[i] != '=')
		{
			if (nameAndValue[i] == '}')
			{
				leftBigPara--;
			}

			if (flags)//等号数量大于零
			{
				value += nameAndValue[i];
			}
			else
			{
				name += nameAndValue[i];
			}

			if (leftBigPara == 0)
			{
				if (nameAndValue[i] == '}')
				{
					i++;
					//value.RemoveAt(0, 1);
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}

				if (flags == 1 && nameAndValue[i] == ',')
				{
					value.RemoveAt(value.Len() - 1, 1);
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}

				if (flags >= 1 && leftBigPara == 0 && nameAndValue[i] == ',' && nameAndValue[i - 1] == '\'')
				{
					value.RemoveAt(value.Len() - 1, 1);
					nameArray.Add(name);
					valueArray.Add(value);
					NaVMap.Add(name, value);
					name.Empty();
					value.Empty();
					leftBigPara = 0;
					flags = 0;
					continue;
				}
			}
			continue;
		}
		else
		{
			if (nameAndValue[i + 1] == '{')
			{
				leftBigPara++;
				if (nameAndValue[i + 2] == '{')
				{
					leftBigPara++;
				}
				if (nameAndValue[i] != '=')
				{
					value += nameAndValue[i];
				}
			}
		}
	}

	//开始set
	for (TFieldIterator<FProperty> PropertyIterator(Object->GetClass()); PropertyIterator; ++PropertyIterator)
	{
		FProperty* Property = *PropertyIterator;
		FString PropertyName = Property->GetName();

		for (int i = 0; i < NaVMap.Num(); i++)
		{
			if (PropertyName == nameArray[i])
			{
				CheckSetType(Property, Object, valueArray[i]);
				break;
			}
		}
	}

	return Object;
}

bool UUnLuaBlueprintLibrary::ScriptExists(const FString& LuaFilePath)
{
	FString RelativeFilePath = FString::Printf(TEXT("%s.lua"), *LuaFilePath);
	FString FullFilePath = GLuaSrcFullPath + RelativeFilePath;
	return IFileManager::Get().FileExists(*FullFilePath);
}

bool UUnLuaBlueprintLibrary::GetRowFromLua(FName LuaTableName, FName RowName, const UScriptStruct* TargetStruct, FTableRowBase& OutRow)
{
	check(0);
	return false;
}

bool UUnLuaBlueprintLibrary::Generic_GetRowFromLua(FName LuaTableName, FName RowName, UScriptStruct* TargetStruct, void* OutRowPtr)
{
	if (TargetStruct)
	{
		UnLua::FLuaRetValues RetValues = UnLua::Call(UnLua::GetState(), "GetRowByName", TargetStruct->GetFName(), LuaTableName, RowName);
		if (RetValues.IsValid() && RetValues.Num() > 0 && RetValues[0].GetType() == LUA_TUSERDATA)
		{
			TargetStruct->CopyScriptStruct(OutRowPtr, RetValues[0]);
			return true;
		}
	}
	return false;
}
