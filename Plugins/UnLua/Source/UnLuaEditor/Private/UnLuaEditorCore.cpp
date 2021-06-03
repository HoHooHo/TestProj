// Tencent is pleased to support the open source community by making UnLua available.
// 
// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the MIT License (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, 
// software distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.

#include "UnLuaPrivate.h"
#include "UnLuaInterface.h"
#include "UnLuaEditorNotificationHelper.h"
#include "UnLuaWidgetString.h"

// engine header
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Engine/Blueprint.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "UMG.h"
#include "Components/RichTextBlock.h"
#include "Interfaces/IPluginManager.h"

extern const char* LUA_DEFAULT_STR_TEMPLATE;
extern const char* LUA_DEFAULT_FUNCTION_TEMPLATE;
extern const char* LUA_TEXTBLOCK_STR_TEMPLATE;
extern const char* LUA_IMAGE_STR_TEMPLATE;
extern const char* LUA_BUTTON_STR_TEMPLATE;
extern const char* LUA_BUTTON_FUNCTION_TEMPLATE;
extern const char* LUA_PROGRESSBAR_STR_TEMPLATE;
extern const char* LUA_RICHTEXTBLOCK_STR_TEMPLATE;
extern const char* LUA_SLIDER_STR_TEMPLATE;
extern const char* LUA_SLIDER_FUNCTION_TEMPLATE;
extern const char* LUA_CHECKBOX_STR_TEMPLATE;
extern const char* LUA_CHECKBOX_FUNCTION_TEMPLATE;
extern const char* LUA_LISTVIEW_STR_TEMPLATE;
extern const char* LUA_COMBOBOXSTRING_STR_TEMPLATE;
extern const char* LUA_COMBOBOXSTRING_FUNCTION_TEMPLATE;
extern const char* LUA_EDITABLETEXT_STR_TEMPLATE;
extern const char* LUA_EDITABLETEXT_FUNCTION_TEMPLATE;
extern const char* LUA_SPINBOX_STR_TEMPLATE;
extern const char* LUA_SPINBOX_FUNCTION_TEMPLATE;
extern const char* LUA_ANIMATION_STR_TEMPLATE;
extern const char* LUA_ANIMATION_FUNCTION_TEMPLATE;
extern const char* LUA_EVENTGRAPHS_FUNCTION_TEMPLATE;

#define LOCTEXT_NAMESPACE "UnLuaEditorCore"


void GenerateVariablesAndFunctions(UBlueprint* Blueprint, FString& Variables, FString& Functions)
{
	UWidgetBlueprint* WidgetBP = Cast<UWidgetBlueprint>(Blueprint);

	if (WidgetBP == nullptr)
	{
		return;
	}

	UClass* Class = Blueprint->GeneratedClass;
	FString ClassName = Class->GetName();

	//TArray<UWidget*> Widgets = WidgetBP->GetAllSourceWidgets();

	TArray<UWidget*> Widgets;
	WidgetBP->WidgetTree->GetAllWidgets(Widgets);

	//Widgets.Sort([](const UWidget& Lhs, const UWidget& Rhs) { return Rhs.GetFName().LexicalLess(Lhs.GetFName()); });

	//WidgetTree
	for (UWidget* Widget : Widgets)
	{
		char* Variable = "";
		char* Function = "";

		if (Widget->bIsVariable)
		{
			FString WidgetName = Widget->GetName();
			if (Widget->IsA<UTextBlock>())
			{
				Variable = const_cast<char*>(LUA_TEXTBLOCK_STR_TEMPLATE);
			}
			else if (Widget->IsA<UImage>())
			{
				Variable = const_cast<char*>(LUA_IMAGE_STR_TEMPLATE);
			}
			else if (Widget->IsA<UButton>())
			{
				Variable = const_cast<char*>(LUA_BUTTON_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_BUTTON_FUNCTION_TEMPLATE);
			}
			else if (Widget->IsA<UProgressBar>())
			{
				Variable = const_cast<char*>(LUA_PROGRESSBAR_STR_TEMPLATE);
			}
			else if (Widget->IsA<URichTextBlock>())
			{
				Variable = const_cast<char*>(LUA_RICHTEXTBLOCK_STR_TEMPLATE);
			}
			else if (Widget->IsA<USlider>())
			{
				Variable = const_cast<char*>(LUA_SLIDER_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_SLIDER_FUNCTION_TEMPLATE);
			}
			else if (Widget->IsA<UCheckBox>())
			{
				Variable = const_cast<char*>(LUA_CHECKBOX_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_CHECKBOX_FUNCTION_TEMPLATE);
			}
			else if (Widget->IsA<UListView>())
			{
				Variable = const_cast<char*>(LUA_LISTVIEW_STR_TEMPLATE);
			}
			else if (Widget->IsA<UComboBoxString>())
			{
				Variable = const_cast<char*>(LUA_COMBOBOXSTRING_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_COMBOBOXSTRING_FUNCTION_TEMPLATE);
			}
			else if (Widget->IsA<UEditableText>()
				|| Widget->IsA<UEditableTextBox>()
				|| Widget->IsA<UMultiLineEditableText>()
				|| Widget->IsA<UMultiLineEditableTextBox>()
				)
			{
				Variable = const_cast<char*>(LUA_EDITABLETEXT_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_EDITABLETEXT_FUNCTION_TEMPLATE);
			}
			else if (Widget->IsA<USpinBox>())
			{
				Variable = const_cast<char*>(LUA_SPINBOX_STR_TEMPLATE);
				Function = const_cast<char*>(LUA_SPINBOX_FUNCTION_TEMPLATE);
			}
			else
			{
				Variable = const_cast<char*>(LUA_UNKNOWN_STR_TEMPLATE);
			}

			Variables += FString(Variable).Replace(TEXT("WIDGET_NAME"), *WidgetName) + TEXT("\n");
			if (FName(Function) != FName(""))
			{
				Functions += FString(Function).Replace(TEXT("WIDGET_NAME"), *WidgetName).Replace(TEXT("CLASS_NAME"), *ClassName) + TEXT("\n\n");
			}
		}
	}

	//Animations
	for (UWidgetAnimation* WidgetAnimation : WidgetBP->Animations)
	{
		FString AnimtionName = WidgetAnimation->GetName();
		Variables += FString(LUA_ANIMATION_STR_TEMPLATE).Replace(TEXT("ANIMATION_NAME"), *AnimtionName) + TEXT("\n");
		Functions += FString(LUA_ANIMATION_FUNCTION_TEMPLATE).Replace(TEXT("ANIMATION_NAME"), *AnimtionName).Replace(TEXT("CLASS_NAME"), *ClassName) + TEXT("\n\n");
	}

	/*
	//EventGraphs
	for (UEdGraph* EdGraph : WidgetBP->EventGraphs)
	{
		FString EventGraphName = EdGraph->GetName();

		FString Params;

		for (UEdGraphNode* EdGraphNode : EdGraph->Nodes)
		{
			for (UEdGraphPin* EdGraphPin : EdGraphNode->Pins)
			{
				if (EEdGraphPinDirection::EGPD_Output == EdGraphPin->Direction && EdGraphPin->PinName != FName("then"))
				{
					Params += EdGraphPin->PinName.ToString() + FString(", ");
				}
			}
		}

		Params = Params.LeftChop(2);
		Functions += FString(LUA_EVENTGRAPHS_FUNCTION_TEMPLATE)
			.Replace(TEXT("EVENTGRAPHS"), *EventGraphName)
			.Replace(TEXT("PARAMS"), *Params)
			.Replace(TEXT("CLASS_NAME"), *ClassName) + TEXT("\n\n");
	}
	*/

	//Lua Common
	Variables += FString(LUA_DEFAULT_STR_TEMPLATE);
	Functions += FString(LUA_DEFAULT_FUNCTION_TEMPLATE).Replace(TEXT("CLASS_NAME"), *ClassName);
}

// create Lua template file for the selected blueprint
bool CreateLuaTemplateFile(UBlueprint *Blueprint, bool bShowFileNotifiction)
{
    if (Blueprint)
    {
        UClass *Class = Blueprint->GeneratedClass;
		
		UClass* GeneratedClass = Blueprint->GeneratedClass;

		bool bImplUnLuaInterface = UKismetSystemLibrary::DoesImplementInterface(GeneratedClass->ClassDefaultObject, UUnLuaInterface::StaticClass());

		if (!bImplUnLuaInterface)
		{
			UE_LOG(LogUnLua, Warning, TEXT("The Blueprint class is not directly add UnLuaInterface!"));
			auto Msg = LOCTEXT("BlueprintIsntInheritanceUnLua","The Blueprint class is not add UnLuaInterface!");
			UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, TEXT(""));
			return false;
		}
        FString ClassName = Class->GetName();
        FString OuterPath = Class->GetPathName();
        int32 LastIndex;
        if (OuterPath.FindLastChar('/', LastIndex))
        {
            OuterPath = OuterPath.Left(LastIndex + 1);
        }
        OuterPath = OuterPath.RightChop(6);         // ignore "/Game/"
        FString FileName = FString::Printf(TEXT("%s%s%s.lua"), *GLuaSrcFullPath, *OuterPath, *ClassName);
        if (FPaths::FileExists(FileName))
        {
            UE_LOG(LogUnLua, Warning, TEXT("Lua file (%s) is already existed!"), *ClassName);
			if (bShowFileNotifiction)
			{
				auto Msg = LOCTEXT("LuaFileIsAlreadtExisted", "Lua file is already existed!");
				UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, FileName,SNotificationItem::ECompletionState::CS_Success);
			}
			system(TCHAR_TO_ANSI(*FileName)); // open file
            return false;
        }

        static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("UnLua"))->GetContentDir();

        FString TemplateName;
        if (Class->IsChildOf(AActor::StaticClass()))
        {
            // default BlueprintEvents for Actor
            TemplateName = ContentDir + TEXT("/ActorTemplate.lua");
        }
        else if (Class->IsChildOf(UUserWidget::StaticClass()))
        {
            // default BlueprintEvents for UserWidget (UMG)
            TemplateName = ContentDir + TEXT("/UserWidgetTemplate.lua");
        }
        else if (Class->IsChildOf(UAnimInstance::StaticClass()))
        {
            // default BlueprintEvents for AnimInstance (animation blueprint)
            TemplateName = ContentDir + TEXT("/AnimInstanceTemplate.lua");
        }
        else if (Class->IsChildOf(UActorComponent::StaticClass()))
        {
            // default BlueprintEvents for ActorComponent
            TemplateName = ContentDir + TEXT("/ActorComponentTemplate.lua");
        }
		else if (Class->IsChildOf(UObject::StaticClass()))
		{
			// default BlueprintEvents for ActorComponent
			TemplateName = ContentDir + TEXT("/ObjectTemplate.lua");
		}

		FString Variables;
		FString Functions;
		GenerateVariablesAndFunctions(Blueprint, Variables, Functions);

        FString Content;
        FFileHelper::LoadFileToString(Content, *TemplateName);
		Content = Content.Replace(TEXT("TemplateName"), *ClassName);
		Content = Content.Replace(TEXT("-- VARIABLES"), *Variables);
		Content = Content.Replace(TEXT("-- FUNCTIONS"), *Functions);
		Content = Content.Replace(TEXT("${date}"), *FDateTime::Now().ToString(TEXT("%Y/%m/%d")));
		Content = Content.Replace(TEXT("${time}"), *FDateTime::Now().ToString(TEXT("%H:%M:%S")));

        bool bSaveStatues = FFileHelper::SaveStringToFile(Content, *FileName);
		if (bShowFileNotifiction)
		{
			auto Msg = LOCTEXT("CreateLuaFile", "Lua file is Created!");
			UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, FileName,SNotificationItem::ECompletionState::CS_Success);
		}
		return bSaveStatues;
    }
    return false;
}

#undef LOCTEXT_NAMESPACE