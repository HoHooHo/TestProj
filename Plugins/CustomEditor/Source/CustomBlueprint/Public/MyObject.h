// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CUSTOMBLUEPRINT_API UMyObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 MyObjInt;

	UPROPERTY(EditAnywhere)
	bool MyObjBool;

	UPROPERTY(EditAnywhere)
	float MyObjFloat;
};
