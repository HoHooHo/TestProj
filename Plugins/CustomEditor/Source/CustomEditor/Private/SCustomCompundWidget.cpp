// Copyright Epic Games, Inc. All Rights Reserved.

#include "SCustomCompundWidget.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SToolTip.h"
#include "UnrealEdMisc.h"
#include "CustomEditor.h"
#include "MyAsset.h"
#include "MyAssetFactory.h"
#include "AssetRegistryModule.h"
#include "PackageTools.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

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
				.OnClicked(this, &SCustomCompundWidget::CreateAsset)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CreateAsset", "Create Asset"))
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
				.OnClicked(this, &SCustomCompundWidget::CreateAssetWithDialog)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CreateAssetWithDialog", "Create Asset With Dialog"))
				]
			]
	];

	ChildSlot
	[
		MainContent
	];
}


//See the UAssetToolsImpl::CreateAsset method in 442 of AssetTools.cpp
FReply SCustomCompundWidget::CreateAsset()
{

	UE_LOG(CustomEditor, Warning, TEXT("CreateAsset"));


	const FString PackagePath = "/Game";
	const FString AssetName = "NewMyAsset";

	/*
	const FString PackageName = UPackageTools::SanitizePackageName(PackagePath + TEXT("/") + AssetName);

	UPackage* Pkg = CreatePackage(*PackageName);
	EObjectFlags Flags = RF_Public | RF_Standalone | RF_Transactional;

	UMyAsset* NewObj = NewObject<UMyAsset>(Pkg, FName(*AssetName), Flags);

	NewObj->AssetInt = 666;
	NewObj->PostEditChange();


	// Notify the asset registry
	FAssetRegistryModule::AssetCreated(NewObj);

	// Mark the package dirty...
	Pkg->MarkPackageDirty();
	*/

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	UObject* NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UMyAsset::StaticClass(), NewObject<UMyAssetFactory>(), FName("Create New Asset"));

	if (NewAsset)
	{
		UMyAsset* MyAsset = Cast<UMyAsset>(NewAsset);
		if (MyAsset)
		{
			MyAsset->AssetInt = 999;
		}
	}

	return FReply::Handled();
}

FReply SCustomCompundWidget::CreateAssetWithDialog()
{
	UE_LOG(CustomEditor, Warning, TEXT("CreateAssetWithDialog"));
	//FName("ContentBrowserNewAsset")

	const FString PackagePath = "/Game";
	const FString AssetName = "NewMyAsset";



	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	UObject* NewAsset = AssetTools.CreateAssetWithDialog(AssetName, PackagePath, UMyAsset::StaticClass(), NewObject<UMyAssetFactory>(), FName("Create New Asset"));

	if (NewAsset)
	{
		UMyAsset* MyAsset = Cast<UMyAsset>(NewAsset);
		if (MyAsset)
		{
			MyAsset->AssetInt = 888;
		}
	}

	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
