// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FCustomEditorButtonsStyle
{
public:
	//启动插件时 初始化并注册 样式信息
	static void Startup();
	//关闭插件时 反注册 样式信息
	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();
	//定义按钮样式时获取样式名字
	static FName GetStyleSetName();

private:
	//初始化样式信息
	static TSharedRef< class FSlateStyleSet > Init();

private:
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};