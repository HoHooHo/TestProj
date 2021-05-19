// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "Factory_MyBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class UFactory_MyBlueprint : public UBlueprintFactory
{
	GENERATED_BODY()
	
public:
	UFactory_MyBlueprint();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);
};
