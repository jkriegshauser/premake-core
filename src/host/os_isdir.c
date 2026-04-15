/**
 * \file   os_isdir.c
 * \brief  Returns true if the specified directory exists.
 * \author Copyright (c) 2002-2008 Jess Perkins and the Premake project
 */

#include <string.h>
#include <sys/stat.h>
#include "premake.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#endif

int os_isdir(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);

	/* empty path is equivalent to ".", must be true */
	if (*path == '\0')
	{
		lua_pushboolean(L, 1);
		return 1;
	}

#if PLATFORM_WINDOWS
	struct _stat buf;
	wchar_t wide_path[PATH_MAX + 1];
	int size = MultiByteToWideChar(CP_UTF8, 0, path, -1, wide_path, PATH_MAX + 1);
	if (size <= 0 || size > PATH_MAX)
	{
		return luaL_error(L, "unable to encode path");
	}
	if (_wstat(wide_path, &buf) == 0)
	{
		int isdir = (buf.st_mode & S_IFDIR) != 0;
		lua_pushboolean(L, isdir);
	}
#else
	struct stat buf;
	if (stat(path, &buf) == 0)
	{
		int isdir = (buf.st_mode & S_IFDIR) != 0;
		lua_pushboolean(L, isdir);
	}
#endif
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}


