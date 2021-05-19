// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory_MyBlueprint.h"
#include "MyBlueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintEditorSettings.h"
#include "BlueprintEditorToolkit/EdGraph_MyBlueprint.h"
#include "BlueprintEditorToolkit/EdGraphSchema_K2_MyBlueprint.h"

#define LOCTEXT_NAMESPACE "CustomBlueprintEditor"

UFactory_MyBlueprint::UFactory_MyBlueprint()
{
	bCreateNew = true;
	bEditAfterNew = true;

	SupportedClass = UMyBlueprint::StaticClass();

	ParentClass = UMyObject::StaticClass();
	BlueprintType = BPTYPE_Normal;
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