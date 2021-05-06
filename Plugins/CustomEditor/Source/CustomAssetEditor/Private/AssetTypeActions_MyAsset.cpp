// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_MyAsset.h"
#include "MyAsset.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FAssetTypeActions_MyAsset::FAssetTypeActions_MyAsset(EAssetTypeCategories::Type InAssetCategory)
	:AssetCategory(InAssetCategory)
{

}

FText FAssetTypeActions_MyAsset::GetName() const
{
	return LOCTEXT("AssetTypeActions_MyAsset", "My Asset");
}

FColor FAssetTypeActions_MyAsset::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FAssetTypeActions_MyAsset::GetSupportedClass() const
{
	return UMyAsset::StaticClass();
}

uint32 FAssetTypeActions_MyAsset::GetCategories()
{
	return AssetCategory | EAssetTypeCategories::Misc;
}

#undef LOCTEXT_NAMESPACE
