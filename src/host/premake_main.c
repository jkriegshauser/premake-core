/**
 * \file   premake_main.c
 * \brief  Program entry point.
 * \author Copyright (c) 2002-2013 Jess Perkins and the Premake project
 */

#include "premake.h"
#if PLATFORM_WINDOWS
#include <shellapi.h>
#include <assert.h>
#endif

#if PLATFORM_WINDOWS
int wmain(int argc, const wchar_t** argv)
#else
int main(int argc, const char** argv)
#endif
{
	lua_State *L;
	int z;

#if PLATFORM_WINDOWS
	/* Set UTF-8 console output for proper string formatting */
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	L = luaL_newstate();
	luaL_openlibs(L);

	z = premake_init(L);
	if (z == OKAY) {
		z = premake_execute(L, argc, argv, "src/_premake_main.lua");
	}

	lua_close(L);
	return z;
}
