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
#include "UnLuaEditorStyle.h"
#include "UnLuaEditorCommands.h"
#include "Animation/AnimBlueprint.h"
#include "AnimationBlueprintEditor.h"
#include "AnimationBlueprintEditorModule.h"
#include "BlueprintEditorModule.h"
#include "ContentBrowserModule.h"
#include "DataTableLUA.h"
#include "Editor.h"
#include "Engine/DataTable.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Interfaces/IMainFrameModule.h"
#include "Interfaces/IPluginManager.h"
#include "IPersonaToolkit.h"
#include "Kismet2/DebuggerCommands.h"

#define LOCTEXT_NAMESPACE "FUnLuaEditorModule"

extern bool CreateLuaTemplateFile(UBlueprint *Blueprint,bool bShowFileNotifiction=false);

// copy dependency file to plugin's content dir
static bool CopyDependencyFile(const TCHAR *FileName)
{
    static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("UnLua"))->GetContentDir();
    FString SrcFilePath = ContentDir / FileName;
    FString DestFilePath = GLuaSrcFullPath / FileName;
    bool bSuccess = IFileManager::Get().FileExists(*DestFilePath);
    if (!bSuccess)
    {
        bSuccess = IFileManager::Get().FileExists(*SrcFilePath);
        if (!bSuccess)
        {
            return false;
        }

        uint32 CopyResult = IFileManager::Get().Copy(*DestFilePath, *SrcFilePath, 1, true);
        if (CopyResult != COPY_OK)
        {
            return false;
        }
    }
    return true;
}

class FUnLuaEditorModule : public IModuleInterface
{
public:
    FUnLuaEditorModule()
        : bIsPIE(false)
    {
    }

    virtual void StartupModule() override
    {
		// add PKG_UncookedOnly flag to UnLuaEditor package to avoid UK2Node_GetRowFromLua compile warning
		if (UPackage* Package = Cast<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), nullptr, TEXT("/Script/UnLuaEditor"), false, false)))
		{
			Package->SetPackageFlags(PKG_UncookedOnly);
		}

        Style = MakeShareable(new FUnLuaEditorStyle);

        FUnLuaEditorCommands::Register();
        SetupContentBrowserContextMenuExtender();

        // register delegates
        OnPostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FUnLuaEditorModule::OnPostEngineInit);
        PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddRaw(this, &FUnLuaEditorModule::PostPIEStarted);
        PrePIEEndedHandle = FEditorDelegates::PrePIEEnded.AddRaw(this, &FUnLuaEditorModule::PrePIEEnded);
    }

    virtual void ShutdownModule() override
    {
        FUnLuaEditorCommands::Unregister();
        RemoveContentBrowserContextMenuExtender();

        // unregister delegates
        FCoreDelegates::OnPostEngineInit.Remove(OnPostEngineInitHandle);
        FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
        FEditorDelegates::PrePIEEnded.Remove(PrePIEEndedHandle);
    }

private:
    void OnPostEngineInit()
    {
        FBlueprintEditorModule &BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
        BlueprintEditorModule.GetMenuExtensibilityManager()->GetExtenderDelegates().Add(FAssetEditorExtender::CreateRaw(this, &FUnLuaEditorModule::GetBlueprintToolbarExtender));

        FAnimationBlueprintEditorModule &AnimationBlueprintEditorModule = FModuleManager::LoadModuleChecked<FAnimationBlueprintEditorModule>("AnimationBlueprintEditor");
        AnimationBlueprintEditorModule.GetAllAnimationBlueprintEditorToolbarExtenders().Add(IAnimationBlueprintEditorModule::FAnimationBlueprintEditorToolbarExtender::CreateRaw(this, &FUnLuaEditorModule::GetAnimationBlueprintToolbarExtender));

        IMainFrameModule &MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
        MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FUnLuaEditorModule::OnMainFrameCreationFinished);
    }

    void PostPIEStarted(bool bIsSimulating)
    {
        bIsPIE = true;      // raise PIE flag
    }

    void PrePIEEnded(bool bIsSimulating)
    {
        bIsPIE = false;     // clear PIE flag
    }

    TSharedRef<FExtender> GetBlueprintToolbarExtender(const TSharedRef<FUICommandList> CommandList, const TArray<UObject*> ContextSensitiveObjects)
    {
        // add a button to blueprint editor toolbar
        UBlueprint *Blueprint = ContextSensitiveObjects.Num() < 1 ? nullptr : Cast<UBlueprint>(ContextSensitiveObjects[0]);
        TSharedPtr<class FUICommandList> NewCommandList = MakeShareable(new FUICommandList);
        NewCommandList->MapAction(FUnLuaEditorCommands::Get().CreateLuaTemplate, FExecuteAction::CreateLambda([Blueprint]() { CreateLuaTemplateFile(Blueprint,true); }), FCanExecuteAction());
        TSharedRef<FExtender> ToolbarExtender(new FExtender());
        ToolbarExtender->AddToolBarExtension("Debugging", EExtensionHook::After, NewCommandList, FToolBarExtensionDelegate::CreateRaw(this, &FUnLuaEditorModule::AddToolbarExtension));
        return ToolbarExtender;
    }

    TSharedRef<FExtender> GetAnimationBlueprintToolbarExtender(const TSharedRef<FUICommandList> CommandList, TSharedRef<IAnimationBlueprintEditor> InAnimationBlueprintEditor)
    {
        // add a button to animation blueprint editor toolbar
        UAnimBlueprint *AnimBlueprint = InAnimationBlueprintEditor->GetPersonaToolkit()->GetAnimBlueprint();
        TSharedPtr<class FUICommandList> NewCommandList = MakeShareable(new FUICommandList);
        NewCommandList->MapAction(FUnLuaEditorCommands::Get().CreateLuaTemplate, FExecuteAction::CreateLambda([AnimBlueprint]() { CreateLuaTemplateFile(AnimBlueprint,true); }), FCanExecuteAction());
        TSharedRef<FExtender> ToolbarExtender(new FExtender());
        ToolbarExtender->AddToolBarExtension("Debugging", EExtensionHook::After, NewCommandList, FToolBarExtensionDelegate::CreateRaw(this, &FUnLuaEditorModule::AddToolbarExtension));
        return ToolbarExtender;
    }

    void OnMainFrameCreationFinished(TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow)
    {
        // register default key input to 'Hotfix' Lua
        FPlayWorldCommands::GlobalPlayWorldActions->MapAction(FUnLuaEditorCommands::Get().HotfixLua, FExecuteAction::CreateLambda([]() { HotfixLua(); }), FCanExecuteAction::CreateRaw(this, &FUnLuaEditorModule::CanHotfixLua));

        // copy dependency files
        bool bSuccess = CopyDependencyFile(TEXT("UnLua.lua"));
        check(bSuccess);
        CopyDependencyFile(TEXT("UnLuaPerformanceTestProxy.lua"));
        check(bSuccess);
    }

    void AddToolbarExtension(FToolBarBuilder &ToolbarBuilder)
    {
        // add a button to export Lua template file
        const FUnLuaEditorCommands &Commands = FUnLuaEditorCommands::Get();
        ToolbarBuilder.BeginSection(NAME_None);
        ToolbarBuilder.AddToolBarButton(Commands.CreateLuaTemplate);
        ToolbarBuilder.EndSection();
    }

    bool CanHotfixLua() const
    {
        return bIsPIE;      // enable Lua 'Hotfix' in PIE
    }

	void SetupContentBrowserContextMenuExtender()
	{
		// Register Content Browser menu extender
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray<FContentBrowserMenuExtender_SelectedAssets>& AssetViewContextMenuExtenders = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		AssetViewContextMenuExtenders.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FUnLuaEditorModule::OnExtendContentBrowserAssetSelectionMenu));
		ContentBrowserAssetExtenderDelegateHandle = AssetViewContextMenuExtenders.Last().GetHandle();
	}

	void RemoveContentBrowserContextMenuExtender()
	{
		if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
		{
			FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");
			TArray<FContentBrowserMenuExtender_SelectedAssets>& AssetViewContextMenuExtenders = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
			AssetViewContextMenuExtenders.RemoveAll([this](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserAssetExtenderDelegateHandle; });
		}
	}

	TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TArray<UDataTable*> DataTables;
		for (const FAssetData& Asset : SelectedAssets)
		{
			if (Asset.AssetClass == UDataTable::StaticClass()->GetFName())
			{
				if (UDataTable* SelectedDataTable = Cast<UDataTable>(Asset.GetAsset()))
				{
					DataTables.Add(SelectedDataTable);
				}
			}
		}

		TSharedRef<FExtender> Extender(new FExtender());
		if (DataTables.Num() > 0)
		{
			Extender->AddMenuExtension("GetAssetActions", EExtensionHook::First, nullptr, FMenuExtensionDelegate::CreateLambda([=](FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.AddMenuEntry(
					LOCTEXT("DataTable_ExportAsLUA", "Export as LUA"),
					LOCTEXT("DataTable_ExportAsLUATooltip", "Export the data table as a file containing LUA data."),
					FSlateIcon(),
					FUIAction(FExecuteAction::CreateRaw(this, &FUnLuaEditorModule::ExportAsLUA, DataTables), FCanExecuteAction()));
			}));
		}

		return Extender;
	}

	void ExportAsLUA(TArray<UDataTable*> DataTables)
	{
		FString ConfigFilePath = IPluginManager::Get().FindPlugin("UnLua")->GetBaseDir() / TEXT("Config") / TEXT("UnLuaEditor.ini");

		for (const UDataTable* DataTable : DataTables)
		{
			FString DataTableName = DataTable->GetName();
			FString ClientExportFile, ServerExportFile;
			TArray<FString> ClientExportFields, ServerExportFields;
			if (!GConfig->GetSingleLineArray(*DataTableName, TEXT("ClientExportFields"), ClientExportFields, ConfigFilePath) ||
				!GConfig->GetSingleLineArray(*DataTableName, TEXT("ServerExportFields"), ServerExportFields, ConfigFilePath) ||
				!GConfig->GetString(*DataTableName, TEXT("ClientExportFile"), ClientExportFile, ConfigFilePath) ||
				!GConfig->GetString(*DataTableName, TEXT("ServerExportFile"), ServerExportFile, ConfigFilePath))
			{
				FFormatNamedArguments Args;
				Args.Add(TEXT("DataTableName"), FText::FromString(DataTableName));
				FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotExportDataTableAsLUA", "Cannot find configuration of DataTable '{DataTableName}' in UnLuaEditor.ini."), Args));
				continue;
			}

			if (ClientExportFields.Num() > 0)
			{
				// Export lua file for client
				FString ClientFilename = FPaths::ProjectDir() / ClientExportFile;
				if (FDataTableExporterLUA(EDataTableExportFlags::UseJsonObjectsForStructs).WriteTable(*DataTable, ClientExportFields, ClientFilename, true))
				{
					UE_LOG(LogUnLua, Log, TEXT("Client LUA file %s exported successfully"), *ClientFilename);
				}
				else
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("DataTableName"), FText::FromString(DataTableName));
					FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotExportDataTableAsLUA", "Cannot export DataTable '{DataTableName}' as LUA for client."), Args));
				}
			}

			if (ServerExportFields.Num() > 0)
			{
				// Export lua file for server
				FString ServerFilename = FPaths::ProjectDir() / ServerExportFile;
				if (FDataTableExporterLUA(EDataTableExportFlags::UseJsonObjectsForStructs).WriteTable(*DataTable, ServerExportFields, ServerFilename, false))
				{
					UE_LOG(LogUnLua, Log, TEXT("Server LUA file %s exported successfully"), *ServerFilename);
				}
				else
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("DataTableName"), FText::FromString(DataTableName));
					FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotExportDataTableAsLUA", "Cannot export DataTable '{DataTableName}' as LUA for server."), Args));
				}
			}
		}

		GConfig->UnloadFile(ConfigFilePath);
	}

	TSharedPtr<ISlateStyle> Style;
	FDelegateHandle ContentBrowserAssetExtenderDelegateHandle;

    FDelegateHandle OnPostEngineInitHandle;
    FDelegateHandle PostPIEStartedHandle;
    FDelegateHandle PrePIEEndedHandle;

    bool bIsPIE;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnLuaEditorModule, UnLuaEditor)
