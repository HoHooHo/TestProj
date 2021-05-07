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
#include "Factory_MyAsset.h"
#include "AssetRegistryModule.h"
#include "PackageTools.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "IAssetTools.h"
#include "FileHelpers.h"
#include "MyAsset.h"

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
		]


		+ SHorizontalBox::Slot()
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.OnClicked(this, &SCustomCompundWidget::ModifySelectedAsset)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModifySelectedAsset", "Modify Selected Asset"))
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
	UObject* NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UMyAsset::StaticClass(), NewObject<UFactory_MyAsset>(), FName("Create New Asset"));

	if (NewAsset)
	{
		UMyAsset* MyAsset = Cast<UMyAsset>(NewAsset);
		if (MyAsset)
		{
			MyAsset->AssetInt = 999;
		}
	}

	UMyAsset* aa = NewObject<UMyAsset>();

	
	GEngine->ForceGarbageCollection();
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
	UObject* NewAsset = AssetTools.CreateAssetWithDialog(AssetName, PackagePath, UMyAsset::StaticClass(), NewObject<UFactory_MyAsset>(), FName("Create New Asset"));

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


FReply SCustomCompundWidget::ModifySelectedAsset()
{
	TArray<FAssetData> AssetDatas;
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
	ContentBrowserSingleton.GetSelectedAssets(AssetDatas);

	if (AssetDatas.Num() > 0)
	{
		UE_LOG(CustomEditor, Warning, TEXT("[%s] is selected!"), *AssetDatas[0].GetFullName());

		UObject* Obj = AssetDatas[0].GetAsset();

		UMyAsset* MyAsset = Cast<UMyAsset>(Obj);
		if (MyAsset)
		{
			MyAsset->AssetInt = 1024;

			TArray<UPackage*> PackageToSave;
			PackageToSave.Add(AssetDatas[0].GetPackage());
			FEditorFileUtils::PromptForCheckoutAndSave(PackageToSave, false, false);
		}
	}
	else
	{
		UE_LOG(CustomEditor, Warning, TEXT("Nothing is selected!"));
	}

	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
