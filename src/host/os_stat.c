/**
 * \file   os_stat.c
 * \brief  Retrieve information about a file.
 * \author Copyright (c) 2011 Jess Perkins and the Premake project
 */

#include "premake.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int os_stat(lua_State* L)
{
	const char* filename = luaL_checkstring(L, 1);

#if PLATFORM_WINDOWS
	struct _stat s;

	wchar_t wide_filename[MAX_PATH + 1];
	int size = MultiByteToWideChar(CP_UTF8, 0, filename, -1, wide_filename, MAX_PATH + 1);
	if (size <= 0 || size > MAX_PATH)
	{
		return luaL_error(L, "unable to encode source path");
	}

	if (_wstat(wide_filename, &s) != 0)
#else
	struct stat s;

	if (stat(filename, &s) != 0)
#endif
	{
		lua_pushnil(L);
		switch (errno)
		{
		case EACCES:
			lua_pushfstring(L, "'%s' could not be accessed", filename);
			break;
		case ENOENT:
			lua_pushfstring(L, "'%s' was not found", filename);
			break;
		default:
			lua_pushfstring(L, "An unknown error %d (%s) occurred while accessing '%s'", errno, strerror(errno), filename);
			break;
		}
		return 2;
	}


	lua_newtable(L);

	lua_pushstring(L, "mtime");
	lua_pushinteger(L, (lua_Integer)s.st_mtime);
	lua_settable(L, -3);

	lua_pushstring(L, "size");
	lua_pushnumber(L, (lua_Number)s.st_size);
	lua_settable(L, -3);

	return 1;
}
