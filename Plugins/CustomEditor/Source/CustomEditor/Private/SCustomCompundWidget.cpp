// Copyright Epic Games, Inc. All Rights Reserved.

#include "SCustomCompundWidget.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SToolTip.h"
#include "UnrealEdMisc.h"

#define LOCTEXT_NAMESPACE "FCustomEditorModule"

SCustomCompundWidget::~SCustomCompundWidget()
{
}

void SCustomCompundWidget::Construct( const FArguments& Args )
{

	TSharedRef<SHorizontalBox> MainContent = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[	
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CreateMyAsset", "Create My Asset"))
				]
			]
		]

		+ SHorizontalBox::Slot()
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CreateMyAsset", "Modify My Asset"))
				]
			]
	];

	ChildSlot
	[
		MainContent
	];
}

#undef LOCTEXT_NAMESPACE
