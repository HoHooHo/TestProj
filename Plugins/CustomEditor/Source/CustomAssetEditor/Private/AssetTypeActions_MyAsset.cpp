// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_MyAsset.h"
#include "MyAsset.h"
#include "MyAssetEditor.h"

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


void FAssetTypeActions_MyAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> InToolkitHost)
{
	EToolkitMode::Type Mode = InToolkitHost.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object = InObjects.CreateConstIterator(); Object; Object++)
	{
		UMyAsset* MyAsset = Cast<UMyAsset>(*Object);
		if (MyAsset)
		{
			TSharedRef<FMyAssetEditor> MyAssetEditor = MakeShareable(new FMyAssetEditor());
			MyAssetEditor->InitGraphAssetEditor(Mode, InToolkitHost, MyAsset);
		}
	}
}

#undef LOCTEXT_NAMESPACE
