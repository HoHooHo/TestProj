// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAssetTools;
class IAssetTypeActions;

DECLARE_LOG_CATEGORY_EXTERN(CustomAsset, All, All);

class FCustomAssetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** All created asset type actions.  Cached here so that we can unregister them during shutdown. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
};
