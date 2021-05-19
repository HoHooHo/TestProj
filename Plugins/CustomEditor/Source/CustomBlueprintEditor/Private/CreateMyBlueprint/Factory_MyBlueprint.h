// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyObject.h"
#include "Factory_MyBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class UFactory_MyBlueprint : public UFactory
{
	GENERATED_BODY()
	
public:
	// The type of blueprint that will be created
	UPROPERTY()
	TEnumAsByte<enum EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY()
	TSubclassOf<UMyObject> ParentClass;

public:
	UFactory_MyBlueprint();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);
};
