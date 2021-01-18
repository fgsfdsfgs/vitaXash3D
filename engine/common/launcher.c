/*
launcher.c - direct xash3d launcher
Copyright (C) 2015 Mittorn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifdef XASH_SINGLE_BINARY

#include <stdlib.h>
#include <string.h>
#ifdef XASH_SDLMAIN
#include "SDL.h"
#endif
#ifdef __vita__
#include <vitasdk.h>
#endif

char szGameDir[128]; // safe place to keep gamedir
int g_iArgc;

void Host_Shutdown( void );
int Host_Main( int szArgc, char **szArgv, const char *szGameDir, int chg, void *callback );

char **g_pszArgv;

void Launcher_ChangeGame( const char *progname );
void Launcher_ChangeGame( const char *progname )
{
	strncpy( szGameDir, progname, sizeof( szGameDir ) - 1 );
	Host_Shutdown( );
	exit( Host_Main( g_iArgc, g_pszArgv, szGameDir, 1, &Launcher_ChangeGame ) );
}
#ifdef XASH_NOCONHOST
#include <windows.h>
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int nShow)
{
	int szArgc;
	char **szArgv;
	LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &szArgc);
	int size, i = 0;
	szArgv = (char**)malloc(szArgc*sizeof(char*));
	for (; i < szArgc; ++i)
	{
		size = wcslen(lpArgv[i]) + 1;
		szArgv[i] = (char*)malloc(size);
		wcstombs(szArgv[i], lpArgv[i], size);
	}
	LocalFree(lpArgv);
	main( szArgc, szArgv );
}
#endif

#ifdef __vita__
SceUInt32 sceUserMainThreadStackSize = 1 * 1024 * 1024;
unsigned int _newlib_heap_size_user = 128 * 1024 * 1024;

int xash_main( int argc, char **argv );

int main( int argc, char **argv )
{
	sceAppUtilInit( &(SceAppUtilInitParam){}, &(SceAppUtilBootParam){} );
	sceCommonDialogSetConfigParam( &(SceCommonDialogConfigParam){} );
	scePowerSetArmClockFrequency( 444 );
	scePowerSetBusClockFrequency( 222 );
	scePowerSetGpuClockFrequency( 222 );
	scePowerSetGpuXbarClockFrequency( 166 );
	sceSysmoduleLoadModule( SCE_SYSMODULE_NET );

	sceIoMkdir( "ux0:/data/xash3d", 0775 );
	sceIoMkdir( "ux0:/data/xash3d/valve", 0775 );
	sceIoMkdir( "ux0:/data/xash3d/valve/save", 0775 ); // crashes without this dir

	xash_main( argc, argv );

	return 0;
}

int xash_main( int argc, char** argv )
#else
int main( int argc, char** argv )
#endif
{
	char gamedir_buf[32] = "";
	const char *gamedir = getenv("XASH3D_GAMEDIR");

	if(!gamedir)
		gamedir = "valve";
	else
	{
		strncpy( gamedir_buf, gamedir, 32 );
		gamedir = gamedir_buf;
	}

#ifdef __EMSCRIPTEN__
	// For some unknown reason emscripten refusing to load libraries later
	Com_LoadLibrary("menu", 0 );
	Com_LoadLibrary("server", 0 );
	Com_LoadLibrary("client", 0 );
#endif

	g_iArgc = argc;
	g_pszArgv = argv;

#if TARGET_OS_IPHONE
	{
		void IOS_LaunchDialog( void );
		IOS_LaunchDialog();
	}
#endif
#ifdef __vita__
	// can't change game after init on the vita
	return Host_Main( g_iArgc, g_pszArgv, gamedir, 0, NULL );
#else
	return Host_Main( g_iArgc, g_pszArgv, gamedir, 0, &Launcher_ChangeGame );
#endif
}

#endif
