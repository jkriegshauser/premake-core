/**
 * \file   os_isfile.c
 * \brief  Returns true if the given file exists on the file system.
 * \author Copyright (c) 2002-2008 Jess Perkins and the Premake project
 */

#include <sys/stat.h>
#include "premake.h"


int os_isfile(lua_State* L)
{
	const char* filename = luaL_checkstring(L, 1);
	lua_pushboolean(L, do_isfile(L, filename));
	return 1;
}

int do_isfile(lua_State *L, const char *filename)
{
#if PLATFORM_WINDOWS
	struct _stat buf;
	wchar_t wide_path[PATH_MAX + 1];

	int size = MultiByteToWideChar(CP_UTF8, 0, filename, -1, wide_path, PATH_MAX + 1);
	if (size <= 0 || size > PATH_MAX)
	{
		return luaL_error(L, "unable to encode filepath");
	}

	if (_wstat(wide_path, &buf) == 0)
	{
		return ((buf.st_mode & S_IFDIR) == 0);
	}
#else
	struct stat buf;

	(void)(L);  /* warning: unused parameter */

	if (stat(filename, &buf) == 0)
	{
		return ((buf.st_mode & S_IFDIR) == 0);
	}
#endif

	return 0;
}
