// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetFactory.h"
#include "MyAsset.h"

UMyAssetFactory::UMyAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UMyAsset::StaticClass();

}

UObject* UMyAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(SupportedClass));

	return NewObject<UMyAsset>(InParent, InClass, InName, Flags);
}