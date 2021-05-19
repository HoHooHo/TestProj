// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBlueprint.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class FAssetTypeActions_MyBlueprint : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_MyBlueprint(EAssetTypeCategories::Type InAssetCategory);


	// IAssetTypeActions Implementation

	/** Returns the name of this type */
	virtual FText GetName() const;

	/** Get the supported class of this type. */
	virtual UClass* GetSupportedClass() const;

	/** Returns the color associated with this type */
	virtual FColor GetTypeColor() const;

	/** Returns the categories that this asset type appears in. The return value is one or more flags from EAssetTypeCategories.  */
	virtual uint32 GetCategories();

	/** Opens the asset editor for the specified objects. If EditWithinLevelEditor is valid, the world-centric editor will be used. */
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>());

private:
	EAssetTypeCategories::Type MyBlueprintCategory;

	/** Returns true if the blueprint is data only */
	bool ShouldUseDataOnlyEditor(const UMyBlueprint* Blueprint) const;
};
