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

int main(int argc, const char** argv)
{
	char* argbuf = NULL;
	lua_State* L;
	int z;

	L = luaL_newstate();
	luaL_openlibs(L);

	z = premake_init(L);
#if PLATFORM_WINDOWS
	/* Set UTF-8 console output for proper string formatting */
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	/* The default is ANSI, but we need UTF-8. So get the Wide arguments and convert them to UTF-8 */
	if (z == OKAY) {
		LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (!wargv) {
			z = !OKAY;
			fprintf(stderr, "Failed to convert command line arguments to wide string: %d\n", GetLastError());
		} else {
			int total = 0, size = 0;
			char* p;
			for (int i = 0; i < argc; ++i) {
				size = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, NULL, 0, NULL, NULL);
				assert(size > 0);
				total += size;
			}
			p = argbuf = malloc(total);
			for (int i = 0; i < argc; ++i) {
				argv[i] = p;
				size = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, p, total, NULL, NULL);
				assert(size > 0);
				p += size;
				total -= size;
			}
			LocalFree(wargv);
		}
	}
#endif
	if (z == OKAY) {
		z = premake_execute(L, argc, argv, "src/_premake_main.lua");
	}

	lua_close(L);

	if (argbuf) {
		free(argbuf);
	}
	return z;
}
