// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CustomEditorButtonsStyle.h"

class FCustomEditorButtonsCommands : public TCommands<FCustomEditorButtonsCommands>
{
public:
	/**
	 * Constructor
	 *
	 * @param InContextName		The name of the context
	 * @param InContextDesc		The localized description of the context
	 * @param InContextParent	Optional parent context.  Bindings are not allowed to be the same between parent and child contexts
	 * @param InStyleSetName	The style set to find the icons in, eg) FCoreStyle::Get().GetStyleSetName()
	 */
	FCustomEditorButtonsCommands()
		: TCommands<FCustomEditorButtonsCommands>(TEXT("CustomButton"), NSLOCTEXT("CustomEditorButtons", "CustomButtonDesc", "Custom Button Desc"), NAME_None, FCustomEditorButtonsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	//ʵ�ֳ����� ���尴ť��ʽ��Ϣ
	virtual void RegisterCommands() override;

public:
	//��Ҫ��ӵİ�ť����Ϣ
	//��ʾ��ͼƬ
	//��ʾ������
	//Tooltips ��
	TSharedPtr< FUICommandInfo > Button1;
	TSharedPtr< FUICommandInfo > Button2;
	TSharedPtr< FUICommandInfo > Button3;
	TSharedPtr< FUICommandInfo > Button4;
};

