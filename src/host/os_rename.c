/**
* \file   os_rename.c
* \brief  Rename a path on Windows.
* \author Copyright (c) 2002-2013 Jess Perkins and the Premake project
*/

#include "premake.h"

#if PLATFORM_WINDOWS

int os_rename(lua_State* L)
{
	const char *fromname = luaL_checkstring(L, 1);
	const char *toname = luaL_checkstring(L, 2);

	wchar_t wide_frompath[MAX_PATH + 1], wide_topath[MAX_PATH + 1];
	int size = MultiByteToWideChar(CP_UTF8, 0, fromname, -1, wide_frompath, MAX_PATH + 1);
	if (size <= 0 || size > MAX_PATH)
	{
		return luaL_error(L, "unable to encode source path");
	}

	size = MultiByteToWideChar(CP_UTF8, 0, toname, -1, wide_topath, MAX_PATH + 1);
	if (size <= 0 || size > MAX_PATH)
	{
		return luaL_error(L, "unable to encode dest path");
	}

	if (MoveFileExW(wide_frompath, wide_topath, MOVEFILE_COPY_ALLOWED))
	{
		lua_pushboolean(L, 1);
		return 1;
	}
	else
	{
		DWORD err = GetLastError();

		char unicodeErr[512 + 1];

		LPWSTR messageBuffer = NULL;
		if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &messageBuffer, 0, NULL) != 0)
		{
			size = WideCharToMultiByte(CP_UTF8, 0, messageBuffer, -1, unicodeErr, sizeof(unicodeErr), NULL, NULL);
			if (size <= 0 || size >= sizeof(unicodeErr))
				strcpy(unicodeErr, "failed to translate error message");

			LocalFree(messageBuffer);
		}
		else
			strcpy(unicodeErr, "failed to get error message");

		lua_pushnil(L);
		lua_pushfstring(L, "%s: %s", fromname, unicodeErr);
		lua_pushinteger(L, err);
		return 3;
	}
}

#endif
