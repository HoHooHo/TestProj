// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorButtonsStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FCustomEditorButtonsStyle::StyleInstance = NULL;

void FCustomEditorButtonsStyle::Startup()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Init();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCustomEditorButtonsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCustomEditorButtonsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CustomEditorButtonsStyle"));
	return StyleSetName;
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

TSharedRef< FSlateStyleSet > FCustomEditorButtonsStyle::Init()
{
	// CustomEditorButtonsStyle ：样式名字，为按钮设置样式时 通过名字获取样式类
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CustomEditorButtonsStyle"));
	//设置图片资源所在路径
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CustomEditor")->GetBaseDir() / TEXT("Resources"));

	//设置样式
	//CustomButton：上下文名字，与 TCommands 中的上下文名字一致
	//Button1：按钮名字，与 FUICommandInfo 的变量名一致
	Style->Set("CustomButton.Button1", new IMAGE_BRUSH(TEXT("ButtonIcon1_88x"), Icon88x88));
	Style->Set("CustomButton.Button2", new IMAGE_BRUSH(TEXT("ButtonIcon2_88x"), Icon88x88));
	Style->Set("CustomButton.Button3", new IMAGE_BRUSH(TEXT("ButtonIcon3_88x"), Icon88x88));
	Style->Set("CustomButton.Button4", new IMAGE_BRUSH(TEXT("ButtonIcon4_88x"), Icon88x88));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FCustomEditorButtonsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}