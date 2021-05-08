// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FCustomEditorButtonsAction
{
public:
	TSharedPtr<class FUICommandList> Commands;

public:
	//��ģ��� StartupModule �е��ô˺�����ɰ�ť��ע��Ȳ���
	void Startup();

	//��ģ��� ShutdownModule �е��ô˺�����ɰ�ť�ķ�ע��Ȳ���
	void Shutdown();

public:
	void OnButton1Clicked();
	void OnButton2Clicked();
	void OnButton3Clicked();
	void OnButton4Clicked();

private:
	//ע�ᰴť�ص��¼�
	void MapActions();
	//����ť��ӵ��༭����
	void RegisterToMenus();

	TSharedRef<class SDockTab> OnSpawnTab2(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<class SDockTab> OnSpawnTab4(const class FSpawnTabArgs& SpawnTabArgs);

};

