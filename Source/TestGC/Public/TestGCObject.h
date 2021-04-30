// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TestGC.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGCObject.h"
#include "TestGCObject.generated.h"

/**
 * 
 */
UCLASS()
class UTestGCObject : public UObject
{
	GENERATED_BODY()


public:
	~UTestGCObject()
	{
		UE_LOG(TestGC, Warning, TEXT("==========  UTestGCObject is Destroy ==========  %s"), *MyName.ToString());
	}

	UPROPERTY()
	UMyGCObject* Obj;

	FName MyName;

	void SetMyName(FName Name)
	{
		MyName = Name;

		if (Obj)
		{
			Obj->MyName = Name;
		}
	}
};


struct FTestGCStruct
{
public:
	FTestGCStruct()
	{
	}

	~FTestGCStruct()
	{
		UE_LOG(TestGC, Warning, TEXT("**********  FTestGCStruct is Destroy **********  %s"), *MyName.ToString());
	}

	FName MyName;

	UPROPERTY()
	UMyGCObject* Obj;


	void SetMyName(FName Name)
	{
		MyName = Name;

		if (Obj)
		{
			Obj->MyName = Name;
		}
	}
};

USTRUCT()
struct FTestGCUStruct
{
	GENERATED_BODY()

public:
	FTestGCUStruct()
	{
	}

	~FTestGCUStruct()
	{
		UE_LOG(TestGC, Warning, TEXT("##########  FTestGCUStruct is Destroy ##########  %s"), *MyName.ToString());
	}

	FName MyName;

	UPROPERTY()
	UMyGCObject* Obj;


	void SetMyName(FName Name)
	{
		MyName = Name;

		if (Obj)
		{
			Obj->MyName = Name;
		}
	}
};