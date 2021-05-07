// Copyright Epic Games, Inc. All Rights Reserved.

#include "DetailCustomization_MyAsset.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

TSharedRef<IDetailCustomization> FDetailCustomization_MyAsset::MakeInstance()
{
	return MakeShareable(new FDetailCustomization_MyAsset());
}

void FDetailCustomization_MyAsset::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory(TEXT("DetailCustomization_MyAsset Category"));

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	CategoryBuilder.AddCustomRow(LOCTEXT("TestRow0", "Test Row0"), false)
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("TextBlock0", "Test Text Block0"))
		]
		.ValueContent()
		[
			SNew(SEditableTextBox)
		];

		CategoryBuilder.AddCustomRow(LOCTEXT("TestRow1", "Test Row1"), false)
			.WholeRowContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("TextBlock1", "Test Text Block1"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
				]
			];

	CategoryBuilder.AddCustomRow(LOCTEXT("TestRow2", "Test Row2"), true)
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("TextBlock2", "Test Text Block2"))
		]
		.ValueContent()
		[
			SNew(SEditableTextBox)
		];

}

#undef LOCTEXT_NAMESPACE