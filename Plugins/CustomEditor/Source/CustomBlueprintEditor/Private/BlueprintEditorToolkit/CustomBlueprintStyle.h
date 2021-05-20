// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * 
 */
class FCustomBlueprintStyle
{
public:
	static void Startup();
	static void Shutdown();

	static void ReloadTextures();
	static FName GetStyleSetName();

	static const FSlateBrush* GetBrush(FName BrushName);
private:
	static TSharedRef< class FSlateStyleSet > Init();

private:
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};
