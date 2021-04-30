// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TestGC.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGCObject.generated.h"

/**
 * 
 */
UCLASS()
class UMyGCObject : public UObject
{
	GENERATED_BODY()
	
public:
	~UMyGCObject()
	{
		UE_LOG(TestGC, Warning, TEXT("**********  UMyGCObject is Destroy *** %s"), *MyName.ToString());
	}


	FName MyName;
};
