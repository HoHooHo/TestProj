// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomEditorButtonsAction.h"
#include "CustomEditorButtonsStyle.h"
#include "CustomEditorButtonsCommands.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"

static const FName CustomEditorButtons_TabName2("CustomEditorButtons_TabName2");
static const FName CustomEditorButtons_TabName4("CustomEditorButtons_TabName4");

#define LOCTEXT_NAMESPACE "CustomEditorButtons"

void FCustomEditorButtonsAction::Startup()
{
	//初始化样式信息
	FCustomEditorButtonsStyle::Startup();
	FCustomEditorButtonsStyle::ReloadTextures();

	//初始化按钮信息
	FCustomEditorButtonsCommands::Register();

	//生成 FUICommandList 实例
	Commands = MakeShareable(new FUICommandList());

	//注册按钮回调事件
	MapActions();

	//将按钮添加到编辑器中
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomEditorButtonsAction::RegisterToMenus));


	//注册全局点击事件
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CustomEditorButtons_TabName2, FOnSpawnTab::CreateRaw(this, &FCustomEditorButtonsAction::OnSpawnTab2))
		.SetDisplayName(LOCTEXT("TabDisplayName2", "Tab Display Name2"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CustomEditorButtons_TabName4, FOnSpawnTab::CreateRaw(this, &FCustomEditorButtonsAction::OnSpawnTab4))
		.SetDisplayName(LOCTEXT("TabDisplayName4", "Tab Display Name4"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FCustomEditorButtonsAction::Shutdown()
{
	UToolMenus::UnregisterOwner(this);
	UToolMenus::UnRegisterStartupCallback(this);

	FCustomEditorButtonsCommands::Unregister();

	FCustomEditorButtonsStyle::Shutdown();
}

void FCustomEditorButtonsAction::MapActions()
{
	Commands->MapAction(
		FCustomEditorButtonsCommands::Get().Button1,
		FExecuteAction::CreateRaw(this, &FCustomEditorButtonsAction::OnButton1Clicked),
		FCanExecuteAction()
	);

	Commands->MapAction(
		FCustomEditorButtonsCommands::Get().Button2,
		FExecuteAction::CreateRaw(this, &FCustomEditorButtonsAction::OnButton2Clicked),
		FCanExecuteAction()
	);

	Commands->MapAction(
		FCustomEditorButtonsCommands::Get().Button3,
		FExecuteAction::CreateRaw(this, &FCustomEditorButtonsAction::OnButton3Clicked),
		FCanExecuteAction()
	);

	Commands->MapAction(
		FCustomEditorButtonsCommands::Get().Button4,
		FExecuteAction::CreateRaw(this, &FCustomEditorButtonsAction::OnButton4Clicked),
		FCanExecuteAction()
	);
}

void FCustomEditorButtonsAction::RegisterToMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		//向 关卡编辑器->菜单栏->Window菜单 添加 MyButton->Button1 和 MyButton->Button2
		{
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("MyButton");
				{
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button1, Commands);
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button2, Commands);

				}
			}
		}

		//向 关卡编辑器->菜单栏 添加 MyMenu 菜单
		{
			UToolMenu* MenuBar = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
			/*
				AddSubMenu(
				const FToolMenuOwner Owner,
				const FName SectionName,
				const FName Name,
				const FText & Label,
				const FText & ToolTip = FText()
			);
			*/
			MenuBar->AddSubMenu(
				"MainMenu",
				NAME_None,
				"MyMenu",
				LOCTEXT("MyMenu", "My Menu"),
				LOCTEXT("MyMenu_ToolTip", "Open the file menu")
			);

			//向 关卡编辑器->菜单栏->MyMenu菜单 添加 MyMenuSection->Button1 和 MyMenuSection->Button2
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.MyMenu");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("MyMenuSection");
				{
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button1, Commands);
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button2, Commands);
				}
			}
		}

		{
			//向 关卡编辑器->工具栏 添加 Button1 和 Button2
			UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
			{
				FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
				{
					FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button1));
					Entry.SetCommandList(Commands);

					FToolMenuEntry& Entry1 = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button2));
					Entry1.SetCommandList(Commands);
					Entry1.InsertPosition.Position = EToolMenuInsertType::First;
				}
			}
		}

		{
			//向 蓝图编辑器->菜单栏->Window菜单 添加 MyButton->Button3 和 MyButton->Button4 和 Test11 和 Test22
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("AssetEditor.BlueprintEditor.MainMenu.Window");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("MyMenuSection");
				{
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button3, Commands);
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button4, Commands);

					Section.AddMenuEntry(
						"Test11",
						LOCTEXT("Test11", "Test11"),
						LOCTEXT("Test11Tooltip", "Test11 Tooltip"),
						FSlateIcon(),
						FToolMenuExecuteAction::CreateLambda
						(
							[](const FToolMenuContext& InMenuContext)
							{
								FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test1111", "Test11 Test Test11 Test11 Test Test Test11"));
							}
						)
					);

					Section.AddEntry(
						FToolMenuEntry::InitMenuEntry(
							"Test22",
							LOCTEXT("Test22", "Test22"),
							LOCTEXT("Test22Tooltip", "Test22 Tooltip"),
							FSlateIcon(),
							FToolMenuExecuteAction::CreateLambda
							(
								[](const FToolMenuContext& InMenuContext)
								{
									FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test2222", "Test22 Test Test22 Test22 Test Test Test22"));
								}
							)
						)
					);
				}
			}
		}

		{
			//向 蓝图编辑器->工具栏 添加 Button3 和 Button4 和 Test33
			UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("AssetEditor.BlueprintEditor.ToolBar");
			{
				FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Asset");
				{
					FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button3));
					Entry.SetCommandList(Commands);

					FToolMenuEntry& Entry1 = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button4));
					Entry1.SetCommandList(Commands);

					FToolMenuEntry& Entry2 = Section.AddEntry(
						FToolMenuEntry::InitToolBarButton(
							"Test33",
							FToolMenuExecuteAction::CreateLambda
							(
								[](const FToolMenuContext& InMenuContext)
								{
									FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Test3333", "Test33 Test Test33 Test33 Test Test Test33"));
								}
							),
							LOCTEXT("Test33", "Test33"),
							LOCTEXT("Test33Tooltip", "Test33 Tooltip")
						)
					);
					Entry2.SetCommandList(Commands);
				}
			}
		}


		{
			//向 静态网格编辑器->菜单栏->Window菜单 添加 MyButton->Button3 和 MyButton->Button4
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("AssetEditor.StaticMeshEditor.MainMenu.Window");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("MyMenuSection");
				{
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button3, Commands);
					Section.AddMenuEntryWithCommandList(FCustomEditorButtonsCommands::Get().Button4, Commands);
				}
			}
		}

		{
			//向 静态网格编辑器->工具栏 添加 Button3 和 Button4
			UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("AssetEditor.StaticMeshEditor.ToolBar");
			{
				FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Asset");
				{
					FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button3));
					Entry.SetCommandList(Commands);

					FToolMenuEntry& Entry1 = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCustomEditorButtonsCommands::Get().Button4));
					Entry1.SetCommandList(Commands);
					Entry1.InsertPosition.Position = EToolMenuInsertType::Default;
				}
			}
		}
	}
}


void FCustomEditorButtonsAction::OnButton1Clicked()
{
	FText DialogText = FText::Format(
		LOCTEXT("DialogText", "On Button[{0}] Clicked"),
		FText::FromString(TEXT("1"))
	);

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FCustomEditorButtonsAction::OnButton2Clicked()
{
	/*
	FText DialogText = FText::Format(
		LOCTEXT("DialogText", "On Button[{0}] Clicked"),
		FText::FromString(TEXT("2"))
	);

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	*/

	//尝试触发全局点击事件
	FGlobalTabmanager::Get()->TryInvokeTab(CustomEditorButtons_TabName2);
}

void FCustomEditorButtonsAction::OnButton3Clicked()
{
	FText DialogText = FText::Format(
		LOCTEXT("DialogText", "On Button[{0}] Clicked"),
		FText::FromString(TEXT("3"))
	);

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FCustomEditorButtonsAction::OnButton4Clicked()
{
	/*
	FText DialogText = FText::Format(
		LOCTEXT("DialogText", "On Button[{0}] Clicked"),
		FText::FromString(TEXT("4"))
	);

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	*/

	//尝试触发全局点击事件
	FGlobalTabmanager::Get()->TryInvokeTab(CustomEditorButtons_TabName4);
}

TSharedRef<SDockTab> FCustomEditorButtonsAction::OnSpawnTab2(const class FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCustomEditorButtonsAction::OnSpawnTab2")),
		FText::FromString(TEXT("CustomEditorButtonsAction.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

TSharedRef<SDockTab> FCustomEditorButtonsAction::OnSpawnTab4(const class FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCustomEditorButtonsAction::OnSpawnTab4")),
		FText::FromString(TEXT("CustomEditorButtonsAction.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

#undef LOCTEXT_NAMESPACE