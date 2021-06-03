//

#include "UnLuaEx.h"
#include "LuaCore.h"
#include "Styling/SlateBrush.h"
#include "MovieSceneSequencePlayer.h"
//#include "Blueprint/UserWidget.h"

// 2020/1/18

static FSoftObjectPath* GetSoftObjectPath(lua_State* L)
{
	int32 NumParams = lua_gettop(L);
	if (NumParams < 1)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	FSoftObjectPath* SoftObjectPath = (FSoftObjectPath*)(GetCppInstanceFast(L, 1));
	if (!SoftObjectPath)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid FSoftObjectPath!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	return SoftObjectPath;
}

static int32 FSoftObjectPath_IsValid(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	lua_pushboolean(L, SoftObjectPath->IsValid());

	return 1;
}
static int32 FSoftObjectPath_ToString(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	const char* msg = TCHAR_TO_ANSI(*SoftObjectPath->ToString());
	lua_pushstring(L, msg);

	return 1;
}
static int32 FSoftObjectPath_GetAssetName(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	const char* msg = TCHAR_TO_ANSI(*SoftObjectPath->GetAssetName());
	lua_pushstring(L, msg);

	return 1;
}
static int32 FSoftObjectPath_GetLongPackageName(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	const char* msg = TCHAR_TO_ANSI(*SoftObjectPath->GetLongPackageName());
	lua_pushstring(L, msg);

	return 1;
}
static int32 FSoftObjectPath_GetAssetPathString(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	const char* msg = TCHAR_TO_ANSI(*SoftObjectPath->GetAssetPathString());
	lua_pushstring(L, msg);

	return 1;
}
/**
 * @see FSoftObjectPath::TryLoad(...)
 */
static int32 FSoftObjectPath_TryLoad(lua_State *L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	UnLua::PushUObject(L, SoftObjectPath->TryLoad());

    return 1;
}

static int32 FSoftObjectPath_Delete(lua_State* L)
{
	FSoftObjectPath* SoftObjectPath = GetSoftObjectPath(L);
	if (!SoftObjectPath)
		return 0;

	SoftObjectPath->~FSoftObjectPath();
	return 0;
}
static const luaL_Reg FSoftObjectPathLib[] =
{
	{ "IsValid", FSoftObjectPath_IsValid },
	{ "ToString", FSoftObjectPath_ToString },
	{ "GetAssetName", FSoftObjectPath_GetAssetName },
	{ "GetLongPackageName", FSoftObjectPath_GetLongPackageName },
	{ "GetAssetPathString", FSoftObjectPath_GetAssetPathString },
	{ "TryLoad", FSoftObjectPath_TryLoad },
	{ "__gc", FSoftObjectPath_Delete },
    //{ "__call", FSoftObjectPath_New },
    { nullptr, nullptr }
};

EXPORT_UNTYPED_CLASS(FSoftObjectPath, false, FSoftObjectPathLib)
IMPLEMENT_EXPORTED_CLASS(FSoftObjectPath)



int32 Global_GetResourceObject(lua_State* L)
{
	int32 nargs = lua_gettop(L);
	if (nargs != 1)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	FSlateBrush* SlateBrush = (FSlateBrush*)(GetCppInstanceFast(L, 1));
	if (!SlateBrush)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid FSlateBrush!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	UnLua::PushUObject(L, SlateBrush->GetResourceObject());

	return 1;
}
int32 Global_SetResourceObject(lua_State* L)
{
	int32 nargs = lua_gettop(L);
	if (nargs != 2)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	FSlateBrush* SlateBrush = (FSlateBrush*)(GetCppInstanceFast(L, 1));
	if (!SlateBrush)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid FSlateBrush!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	UObject* Obj = (UObject*)(GetCppInstanceFast(L, 2));
	if (!Obj)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid UObject!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	SlateBrush->SetResourceObject(Obj);

	return 0;
}

int32 Global_SequenceJumpTo(lua_State* L)
{
	int32 nargs = lua_gettop(L);
	if (nargs != 2)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	UMovieSceneSequencePlayer* seqPlayer = (UMovieSceneSequencePlayer*)(GetCppInstanceFast(L, 1));
	if (!seqPlayer)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid FSlateBrush!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}

	int32 frameNum = lua_tointeger(L, 2);

	FMovieSceneSequencePlaybackParams seqParam(FFrameTime(frameNum),  EUpdatePositionMethod::Jump);
	seqPlayer->SetPlaybackPosition(seqParam);

	return 0;
}

static const luaL_Reg FUE4ExLib[] =
{
	{ "GetResourceObject", Global_GetResourceObject },
	{ "SetResourceObject", Global_SetResourceObject },
	{ "SequenceJumpTo", Global_SequenceJumpTo },
	{ nullptr, nullptr }
};
EXPORT_UNTYPED_CLASS(FUE4Ex, false, FUE4ExLib)
IMPLEMENT_EXPORTED_CLASS(FUE4Ex)
