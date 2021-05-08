// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FCustomEditorButtonsStyle
{
public:
	//�������ʱ ��ʼ����ע�� ��ʽ��Ϣ
	static void Startup();
	//�رղ��ʱ ��ע�� ��ʽ��Ϣ
	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();
	//���尴ť��ʽʱ��ȡ��ʽ����
	static FName GetStyleSetName();

private:
	//��ʼ����ʽ��Ϣ
	static TSharedRef< class FSlateStyleSet > Init();

private:
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};