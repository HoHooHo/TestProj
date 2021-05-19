// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_MyBlueprint.h"
#include "MyBlueprint.h"
#include "BlueprintEditorModule.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

FAssetTypeActions_MyBlueprint::FAssetTypeActions_MyBlueprint(EAssetTypeCategories::Type InAssetCategory)
	: MyBlueprintCategory(InAssetCategory)
{
}

FText FAssetTypeActions_MyBlueprint::GetName() const
{
	return LOCTEXT("AssetTypeActions_MyBlueprint_Name", "MyBlueprint");
}

UClass* FAssetTypeActions_MyBlueprint::GetSupportedClass() const
{
	return UMyBlueprint::StaticClass();
}

FColor FAssetTypeActions_MyBlueprint::GetTypeColor() const
{
	return FColor::Red;
}

uint32 FAssetTypeActions_MyBlueprint::GetCategories()
{
	return EAssetTypeCategories::Blueprint | MyBlueprintCategory;
}

void FAssetTypeActions_MyBlueprint::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor /* = TSharedPtr<IToolkitHost>() */)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Object : InObjects)
	{
		if (UMyBlueprint* Blueprint = Cast<UMyBlueprint>(Object))
		{
			bool bLetOpen = true;

			if (!Blueprint->SkeletonGeneratedClass || !Blueprint->GeneratedClass)
			{
				bLetOpen = EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("FailedToLoadMyBlueprintWithContinue", "MyBlueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed! Do you want to continue (it can crash the editor)?"));
			}

			if (bLetOpen)
			{
				FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");

				TSharedRef<IBlueprintEditor> NewBlueprintEditor = BlueprintEditorModule.CreateBlueprintEditor(Mode, EditWithinLevelEditor, Blueprint, ShouldUseDataOnlyEditor(Blueprint));
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadMyBlueprint", "MyBlueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed!"));
		}
	}
}


bool FAssetTypeActions_MyBlueprint::ShouldUseDataOnlyEditor(const UMyBlueprint* Blueprint) const
{
	return FBlueprintEditorUtils::IsDataOnlyBlueprint(Blueprint)
		&& !FBlueprintEditorUtils::IsLevelScriptBlueprint(Blueprint)
		&& !FBlueprintEditorUtils::IsInterfaceBlueprint(Blueprint)
		&& !Blueprint->bForceFullEditor
		&& !Blueprint->bIsNewlyCreated;
}

#undef LOCTEXT_NAMESPACE