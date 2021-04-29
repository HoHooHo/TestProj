// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetFactory.h"
#include "MyAsset.h"
#include "AssetTypeCategories.h"

UMyAssetFactory::UMyAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UMyAsset::StaticClass();

}

//uint32 UMyAssetFactory::GetMenuCategories() const
//{
//	return EAssetTypeCategories::Misc;
//}

UObject* UMyAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(SupportedClass));

	return NewObject<UMyAsset>(InParent, InClass, InName, Flags);
}