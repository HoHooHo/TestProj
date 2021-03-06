// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_MyAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_MyAsset(EAssetTypeCategories::Type InAssetCategory);

	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> InToolkitHost /* = TSharedPtr<IToolkitHost>() */) override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
