/**
 * \file   os_mkdir.c
 * \brief  Create a subdirectory.
 * \author Copyright (c) 2002-2008 Jess Perkins and the Premake project
 */

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "premake.h"

#if PLATFORM_WINDOWS
#include <direct.h>
#include <errno.h>
#define alloca _alloca
#endif

int do_mkdir(const char* path)
{
	int i, length, s;
#if PLATFORM_WINDOWS
	struct _stat sb;
	wchar_t wpath[MAX_PATH + 1];
	int size = MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH + 1);
	if (size <= 0 || size > MAX_PATH)
		return 0;  /* unable to encode path */
	s = _wstat(wpath, &sb);
#else
	struct stat sb;
	s = stat(path, &sb);
#endif

	// if it already exists, return.
	if (s == 0)
		return 1;

	// find the parent folder name.
	length = (int)strlen(path);
	for (i = length - 1; i >= 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
			break;
	}

	// if we found one, recursively create it.
	if (i > 0)
	{
		char* sub_path = alloca(i + 2); /* null terminator plus trailing slash on Windows */

		memcpy(sub_path, path, i);
		sub_path[i] = '\0';

#if PLATFORM_WINDOWS
		if (sub_path[i - 1] == ':')
		{
			sub_path[i + 0] = '/';
			sub_path[i + 1] = '\0';
		}
#endif

		if (!do_mkdir(sub_path))
			return 0;
	}

	// now finally create the actual folder we want.
#if PLATFORM_WINDOWS
	return _wmkdir(wpath) == 0;
#else
	return  mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}


int os_mkdir(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);

	int z = do_mkdir(path);
	if (!z)
	{
		lua_pushnil(L);
		lua_pushfstring(L, "unable to create directory '%s'", path);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

