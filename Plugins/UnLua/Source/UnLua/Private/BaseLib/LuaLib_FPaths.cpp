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

#include "UnLuaEx.h"
#include "LuaCore.h"
#include "LuaDynamicBinding.h"

static int32 FPaths_Combine(lua_State *L)
{
	int32 NumParams = lua_gettop(L);
	FString FinalPath;
	for (int32 index = 1; index <= NumParams; ++index)
	{
		FString arg = luaL_tolstring(L, index, nullptr);
		if (FinalPath.IsEmpty())
		{
			if (!arg.IsEmpty())
			{
				FinalPath = arg;
				continue;
			}

		}
		if (!arg.IsEmpty())
			FinalPath = FPaths::Combine(FinalPath, arg);
	}
	lua_pushstring(L, TCHAR_TO_ANSI(*FinalPath));
	return 1;
}

static int32 FPaths_ProjectScriptDir(lua_State *L)
{
	int32 NumParams = lua_gettop(L);
	FString Result = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Script"));

	FTCHARToUTF8 UTF8CHAR_Ins(*Result);
	lua_pushlstring(L, (ANSICHAR*)(UTF8CHAR_Ins.Get()), UTF8CHAR_Ins.Length());
	return 1;
}

static int32 FPaths_ScriptExists(lua_State* L)
{
	int32 NumParams = lua_gettop(L);
	if (NumParams < 1)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	const char* ModuleName = lua_tostring(L, 1);
	if (!ModuleName)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid module name!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	FString FileName(ANSI_TO_TCHAR(ModuleName));
	FileName.ReplaceInline(TEXT("."), TEXT("/"));
	FString RelativeFilePath = FString::Printf(TEXT("%s.lua"), *FileName);

	FString FullFilePath = GLuaSrcFullPath + RelativeFilePath;
	FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FString ProjectPersistentDownloadDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir());
	if (!ProjectPersistentDownloadDir.EndsWith("/"))
	{
		ProjectPersistentDownloadDir.Append("/");
	}
	FString RealFilePath = FullFilePath.Replace(*ProjectDir, *ProjectPersistentDownloadDir);
	if (IFileManager::Get().FileExists(*RealFilePath))
	{
		lua_pushboolean(L, 1);
		return 1;
	}
	if (IFileManager::Get().FileExists(*FullFilePath))
	{
		lua_pushboolean(L, 1);
		return 1;
	}

	lua_pushboolean(L, 0);
	return 0;
}

static const luaL_Reg FPathsLib[] =
{
	{ "Combine", FPaths_Combine },
	{ "ProjectScriptDir",FPaths_ProjectScriptDir},
	{ "ScriptExists", FPaths_ScriptExists },
	{ nullptr, nullptr }
};

// Export Actor Component
BEGIN_EXPORT_CLASS(FPaths)
	ADD_STATIC_FUNCTION_EX("EngineDir", FString, EngineDir)
	ADD_STATIC_FUNCTION_EX("EngineUserDir", FString, EngineUserDir)
	ADD_STATIC_FUNCTION_EX("EngineContentDir", FString, EngineContentDir)
	ADD_STATIC_FUNCTION_EX("EngineConfigDir", FString, EngineConfigDir)
	ADD_STATIC_FUNCTION_EX("EngineSavedDir", FString, EngineSavedDir)
	ADD_STATIC_FUNCTION_EX("EnginePluginsDir", FString, EnginePluginsDir)
	// ADD_STATIC_FUNCTION_EX("PlatformExtensionsDir", FString, PlatformExtensionsDir)
	ADD_STATIC_FUNCTION_EX("RootDir", FString, RootDir)
	ADD_STATIC_FUNCTION_EX("ProjectDir", FString, ProjectDir)
	ADD_STATIC_FUNCTION_EX("ProjectUserDir", FString, ProjectUserDir)
	ADD_STATIC_FUNCTION_EX("ProjectContentDir", FString, ProjectContentDir)
	ADD_STATIC_FUNCTION_EX("ProjectConfigDir", FString, ProjectConfigDir)
#if ENGINE_MINOR_VERSION < 25
	ADD_STATIC_FUNCTION_EX("ProjectSavedDir", FString, ProjectSavedDir)
#else
	ADD_STATIC_FUNCTION_EX("ProjectSavedDir", const FString&, ProjectSavedDir)
#endif
	ADD_STATIC_FUNCTION_EX("ProjectIntermediateDir", FString, ProjectIntermediateDir)
	ADD_STATIC_FUNCTION_EX("ProjectPluginsDir", FString, ProjectPluginsDir)
	ADD_STATIC_FUNCTION_EX("ProjectLogDir", FString, ProjectLogDir)
	ADD_STATIC_FUNCTION_EX("GetExtension", FString, GetExtension, const FString&, bool)
	ADD_STATIC_FUNCTION_EX("Split", void, Split,const FString&, FString&, FString&, FString&)
	ADD_STATIC_FUNCTION_EX("FileExists",bool, FileExists,const FString&)
	ADD_STATIC_FUNCTION_EX("DirectoryExists", bool, DirectoryExists, const FString&)
	ADD_STATIC_FUNCTION_EX("ConvertRelativePathToFull",FString, ConvertRelativePathToFull,const FString&)
	ADD_LIB(FPathsLib)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(FPaths)

