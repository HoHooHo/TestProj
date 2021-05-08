// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FCustomEditorButtonsAction
{
public:
	TSharedPtr<class FUICommandList> Commands;

public:
	//在模块的 StartupModule 中调用此函数完成按钮的注册等操作
	void Startup();

	//在模块的 ShutdownModule 中调用此函数完成按钮的反注册等操作
	void Shutdown();

public:
	void OnButton1Clicked();
	void OnButton2Clicked();
	void OnButton3Clicked();
	void OnButton4Clicked();

private:
	//注册按钮回调事件
	void MapActions();
	//将按钮添加到编辑器中
	void RegisterToMenus();

	TSharedRef<class SDockTab> OnSpawnTab2(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<class SDockTab> OnSpawnTab4(const class FSpawnTabArgs& SpawnTabArgs);

};

