// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomBlueprintStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FCustomBlueprintStyle::StyleInstance = NULL;

void FCustomBlueprintStyle::Startup()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Init();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCustomBlueprintStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCustomBlueprintStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CustomBlueprintStyle"));
	return StyleSetName;
}

const FSlateBrush* FCustomBlueprintStyle::GetBrush(FName BrushName)
{
	if (StyleInstance.IsValid())
	{
		return StyleInstance->GetBrush(BrushName);
	}

	return nullptr;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon88x88(88.0f, 88.0f);

TSharedRef< FSlateStyleSet > FCustomBlueprintStyle::Init()
{
	// CustomEditorButtonsStyle ：样式名字，为按钮设置样式时 通过名字获取样式类
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CustomBlueprintStyle"));
	//设置图片资源所在路径
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CustomEditor")->GetBaseDir() / TEXT("Resources"));

	//设置样式
	//CustomButton：上下文名字，与 TCommands 中的上下文名字一致
	//Button1：按钮名字，与 FUICommandInfo 的变量名一致
	Style->Set("CustomBlueprint.Mode1Icon", new IMAGE_BRUSH(TEXT("ModeIcon1_88x"), Icon40x40));
	Style->Set("CustomBlueprint.Mode2Icon", new IMAGE_BRUSH(TEXT("ModeIcon2_88x"), Icon40x40));

	Style->Set("CustomBlueprint.Mode1Icon.Small", new IMAGE_BRUSH(TEXT("ModeIcon1_88x"), Icon20x20));
	Style->Set("CustomBlueprint.Mode2Icon.Small", new IMAGE_BRUSH(TEXT("ModeIcon2_88x"), Icon20x20));

	Style->Set("CustomBlueprint.PipelineSeparator", new BOX_BRUSH(TEXT("Modes/PipelineSeparator"), FMargin(15.0f / 16.0f, 20.0f / 20.0f, 1.0f / 16.0f, 0.0f / 20.0f), FLinearColor(1, 1, 1, 0.5f)));


	Style->Set("CustomeBlueprint.DetailsTabIcon", new IMAGE_BRUSH(TEXT("Icon01"), Icon16x16));


	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FCustomBlueprintStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}