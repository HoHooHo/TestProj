// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Factory_MyAsset.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETEDITOR_API UFactory_MyAsset : public UFactory
{
	GENERATED_BODY()
	
public:
	UFactory_MyAsset();

	//virtual uint32 GetMenuCategories() const override;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
