/**
 * \file   os_remove.c
 * \brief  Remove a file on Windows.
 * \author Copyright (c) 2002-2013 Jess Perkins and the Premake project
 */

#include "premake.h"

#if PLATFORM_WINDOWS

int os_remove(lua_State* L)
{
	const char* filename = luaL_checkstring(L, 1);

	wchar_t wide_path[MAX_PATH + 1];
	int size = MultiByteToWideChar(CP_UTF8, 0, filename, -1, wide_path, MAX_PATH + 1);
	if (size <= 0 || size > MAX_PATH)
	{
		return luaL_error(L, "unable to encode path");
	}

	if (DeleteFileW(wide_path))
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
		lua_pushfstring(L, "%s: %s", filename, unicodeErr);
		lua_pushinteger(L, err);
		return 3;
	}
}

#endif
