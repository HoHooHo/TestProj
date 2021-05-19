// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_MyBlueprint.h"
#include "MyBlueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintEditorSettings.h"
#include "BlueprintEditorToolkit/EdGraph_MyBlueprint.h"
#include "BlueprintEditorToolkit/EdGraphSchema_K2_MyBlueprint.h"

#include "SlateOptMacros.h"
#include "ClassViewerFilter.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"



#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"


// ------------------------------------------------------------------------------
// Dialog to configure creation properties
// ------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

class SMyBlueprintCreateDialog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyBlueprintCreateDialog){}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs)
	{
		bOkClicked = false;
		ParentClass = UMyObject::StaticClass();

		ChildSlot
			[
				SNew(SBorder)
				.Visibility(EVisibility::Visible)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
				[
					SNew(SBox)
					.Visibility(EVisibility::Visible)
					.WidthOverride(500.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.FillHeight(1)
						[
							SNew(SBorder)
							.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
							.Content()
							[
								SAssignNew(ParentClassContainer, SVerticalBox)
							]
						]

						// Ok/Cancel buttons
						+ SVerticalBox::Slot()
						.AutoHeight()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Bottom)
						.Padding(8)
						[
							SNew(SUniformGridPanel)
							.SlotPadding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
							.MinDesiredSlotWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
							.MinDesiredSlotHeight(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
							+ SUniformGridPanel::Slot(0, 0)
							[
								SNew(SButton)
								.HAlign(HAlign_Center)
								.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
								.OnClicked(this, &SMyBlueprintCreateDialog::OkClicked)
								.Text(LOCTEXT("CreateMyBlueprintOk", "OK"))
							]
							+ SUniformGridPanel::Slot(1, 0)
							[
								SNew(SButton)
								.HAlign(HAlign_Center)
								.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
								.OnClicked(this, &SMyBlueprintCreateDialog::CancelClicked)
								.Text(LOCTEXT("CreateMyBlueprintCancel", "Cancel"))
							]
						]
					]
				]
			];

			MakeParentClassPicker();
		}

	/** Sets properties for the supplied MyBlueprintFactory */
	bool ConfigureProperties(TWeakObjectPtr<UFactory_MyBlueprint> InFactory_MyBlueprint)
	{
		Factory_MyBlueprint = InFactory_MyBlueprint;

		TSharedRef<SWindow> Window = SNew(SWindow)
			.Title(LOCTEXT("CreateMyBlueprintOptions", "Create MyBlueprint"))
			.ClientSize(FVector2D(400, 700))
			.SupportsMinimize(false).SupportsMaximize(false)
			[
				AsShared()
			];

		PickerWindow = Window;

		GEditor->EditorAddModalWindow(Window);
		Factory_MyBlueprint.Reset();

		return bOkClicked;
	}

private:
	class FMyBlueprintParentFilter : public IClassViewerFilter
	{
	public:
		/** All children of these classes will be included unless filtered out by another setting. */
		TSet< const UClass* > AllowedChildrenOfClasses;

		FMyBlueprintParentFilter() {}

		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			// If it appears on the allowed child-of classes list (or there is nothing on that list)
			return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			// If it appears on the allowed child-of classes list (or there is nothing on that list)
			return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
		}
	};

	/** Creates the combo menu for the parent class */
	void MakeParentClassPicker()
	{
		// Load the classviewer module to display a class picker
		FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

		// Fill in options
		FClassViewerInitializationOptions Options;
		Options.Mode = EClassViewerMode::ClassPicker;

		// Only allow parenting to base blueprints.
		Options.bIsBlueprintBaseOnly = true;

		TSharedPtr<FMyBlueprintParentFilter> Filter = MakeShareable(new FMyBlueprintParentFilter);

		// All child child classes of UMyObject are valid.
		Filter->AllowedChildrenOfClasses.Add(UMyObject::StaticClass());
		Options.ClassFilter = Filter;

		ParentClassContainer->ClearChildren();
		ParentClassContainer->AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ParentClass", "Parent Class:"))
				.ShadowOffset(FVector2D(1.0f, 1.0f))
			];

		ParentClassContainer->AddSlot()
			[
				ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateSP(this, &SMyBlueprintCreateDialog::OnClassPicked))
			];
	}

	/** Handler for when a parent class is selected */
	void OnClassPicked(UClass* ChosenClass)
	{
		ParentClass = ChosenClass;
	}

	/** Handler for when ok is clicked */
	FReply OkClicked()
	{
		if (Factory_MyBlueprint.IsValid())
		{
			Factory_MyBlueprint->BlueprintType = BPTYPE_Normal;
			Factory_MyBlueprint->ParentClass = ParentClass.Get();
		}

		CloseDialog(true);

		return FReply::Handled();
	}

	void CloseDialog(bool bWasPicked = false)
	{
		bOkClicked = bWasPicked;
		if (PickerWindow.IsValid())
		{
			PickerWindow.Pin()->RequestDestroyWindow();
		}
	}

	/** Handler for when cancel is clicked */
	FReply CancelClicked()
	{
		CloseDialog();
		return FReply::Handled();
	}

	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
	{
		if (InKeyEvent.GetKey() == EKeys::Escape)
		{
			CloseDialog();
			return FReply::Handled();
		}
		return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
	}

private:
	/** The factory for which we are setting up properties */
	TWeakObjectPtr<UFactory_MyBlueprint> Factory_MyBlueprint;

	/** A pointer to the window that is asking the user to select a parent class */
	TWeakPtr<SWindow> PickerWindow;

	/** The container for the Parent Class picker */
	TSharedPtr<SVerticalBox> ParentClassContainer;

	/** The selected class */
	TWeakObjectPtr<UClass> ParentClass;

	/** True if Ok was clicked */
	bool bOkClicked;
};

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


/*------------------------------------------------------------------------------
	UFactory_MyBlueprint implementation.
------------------------------------------------------------------------------*/

UFactory_MyBlueprint::UFactory_MyBlueprint()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UMyBlueprint::StaticClass();

	ParentClass = UMyObject::StaticClass();
	BlueprintType = BPTYPE_Normal;
}


bool UFactory_MyBlueprint::ConfigureProperties()
{
	TSharedRef<SMyBlueprintCreateDialog> Dialog = SNew(SMyBlueprintCreateDialog);
	return Dialog->ConfigureProperties(this);
}


UObject* UFactory_MyBlueprint::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, NAME_None);
}

UObject* UFactory_MyBlueprint::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UMyBlueprint::StaticClass()));

	if ((ParentClass == nullptr) || !ParentClass->IsChildOf(UMyObject::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));

		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotCreateBlueprintFromClass", "Cannot create a blueprint based on the class '{ClassName}'."), Args));
		return nullptr;
	}
	else
	{
		UMyBlueprint* NewBP = CastChecked<UMyBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BlueprintType, UMyBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));

		if (NewBP)
		{
			UMyBlueprint* RootBP = NewBP->FindRootMyBlueprint();

			if (RootBP == nullptr)
			{
				UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(NewBP, TEXT("MyBlueprint Graph"), UEdGraph_MyBlueprint::StaticClass(), UEdGraphSchema_K2_MyBlueprint::StaticClass());

#if WITH_EDITOR
				if (NewBP->UbergraphPages.Num() > 0)
				{
					FBlueprintEditorUtils::RemoveGraphs(NewBP, NewBP->UbergraphPages);
				}
#endif
				FBlueprintEditorUtils::AddUbergraphPage(NewBP, NewGraph);
				NewBP->LastEditedDocuments.Add(NewGraph);
				NewGraph->bAllowDeletion = false;

				UBlueprintEditorSettings* Settings = GetMutableDefault<UBlueprintEditorSettings>();
				if (Settings && Settings->bSpawnDefaultBlueprintNodes)
				{
					int32 NodePositionY = 0;
					//FKismetEditorUtilities::AddDefaultEventNode(NewBP, NewGraph, FName(TEXT("ReceiveBeginPlay")), AActor::StaticClass(), NodePositionY);
				}
			}
		}

		return NewBP;
	}
}

#undef LOCTEXT_NAMESPACE