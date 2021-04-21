// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyAsset.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSET_API UMyAsset : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int AssetInt;


};
