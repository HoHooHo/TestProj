// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSET_API UMyAssetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UMyAssetFactory();

	//virtual uint32 GetMenuCategories() const override;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
