// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_MyAsset.h"
#include "MyAsset.h"
#include "AssetTypeCategories.h"

UFactory_MyAsset::UFactory_MyAsset()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UMyAsset::StaticClass();

}

//uint32 UFactory_MyAsset::GetMenuCategories() const
//{
//	return EAssetTypeCategories::Misc;
//}

UObject* UFactory_MyAsset::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(SupportedClass));

	return NewObject<UMyAsset>(InParent, InClass, InName, Flags);
}