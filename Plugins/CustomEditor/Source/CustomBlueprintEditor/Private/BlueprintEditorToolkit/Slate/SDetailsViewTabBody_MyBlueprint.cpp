// Copyright SinceTimes, Inc. All Rights Reserved.

#include "SDetailsViewTabBody_MyBlueprint.h"
#include "../AssetEditorToolkit_MyBlueprint.h"

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

void SDetailsViewTabBody_MyBlueprint::Construct(const FArguments& InArgs, TSharedPtr<class FAssetEditorToolkit_MyBlueprint> InAssetEditor)
{

	/** Default constructor */
	/*FDetailsViewArgs(const bool InUpdateFromSelection = false
		, const bool InLockable = false
		, const bool InAllowSearch = true
		, const ENameAreaSettings InNameAreaSettings = ActorsUseNameArea
		, const bool InHideSelectionTip = false
		, FNotifyHook * InNotifyHook = NULL
		, const bool InSearchInitialKeyFocus = false
		, FName InViewIdentifier = NAME_None)*/

	FDetailsViewArgs DetailsViewArgs(
		/*InUpdateFromSelection = */false,
		/*InLockable =*/ false,
		/*InAllowSearch =*/ true,
		/*InNameAreaSettings = */ FDetailsViewArgs::HideNameArea,
		/*InHideSelectionTip = */ false,
		/*InNotifyHook = */ nullptr,
		/*InSearchInitialKeyFocus = */ false,
		/*InViewIdentifier = */ NAME_None
	);

	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	PropertyView = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			PropertyView.ToSharedRef()
		]
	];
}

#undef LOCTEXT_NAMESPACE