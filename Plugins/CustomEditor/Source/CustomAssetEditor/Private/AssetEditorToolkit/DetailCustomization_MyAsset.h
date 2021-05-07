// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IDetailRootObjectCustomization.h"


class FDetailCustomization_MyAsset : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};

/*
class FDetailRootObjectCustomization_MyAsset : public IDetailRootObjectCustomization
{
public:
};
*/