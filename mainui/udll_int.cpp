/*
dll_int.cpp - dll entry point
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll.h"
#include "basemenu.h"
#include "utils.h"

ui_enginefuncs_t	g_ui_engfuncs;
ui_textfuncs_t	g_ui_textfuncs;
ui_globalvars_t	*ui_gpGlobals;
CMenu gMenu;

static UI_FUNCTIONS gFunctionTable = 
{
	UI_VidInit,
	UI_Init,
	UI_Shutdown,
	UI_UpdateMenu,
	UI_KeyEvent,
	UI_MouseMove,
	UI_SetActiveMenu,
	UI_AddServerToList,
	UI_GetCursorPos,
	UI_SetCursorPos,
	UI_ShowCursor,
	UI_CharEvent,
	UI_MouseInRect,
	UI_IsVisible,
	UI_CreditsActive,
	UI_FinalCredits
};

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

//=======================================================================
//			GetApi
//=======================================================================
extern "C" EXPORT int GetMenuAPI(UI_FUNCTIONS *pFunctionTable, ui_enginefuncs_t* pEngfuncsFromEngine, ui_globalvars_t *pGlobals)
{
	if( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return FALSE;
	}

	// copy HUD_FUNCTIONS table to engine, copy engfuncs table from engine
	memcpy( pFunctionTable, &gFunctionTable, sizeof( UI_FUNCTIONS ));
	memcpy( &g_ui_engfuncs, pEngfuncsFromEngine, sizeof( ui_enginefuncs_t ));
	memset( &g_ui_textfuncs, 0, sizeof( ui_textfuncs_t ));

	ui_gpGlobals = pGlobals;

	return TRUE;
}

extern "C" EXPORT int GiveTextAPI( ui_textfuncs_t* pTextfuncsFromEngine )
{
	if( !pTextfuncsFromEngine )
	{
		return FALSE;
	}

	// copy HUD_FUNCTIONS table to engine, copy engfuncs table from engine
	memcpy( &g_ui_textfuncs, pTextfuncsFromEngine, sizeof( ui_textfuncs_t ));

	return TRUE;
}

extern "C" EXPORT void AddTouchButtonToList( const char *name, const char *texture, const char *command, unsigned char *color, int flags )
{
	UI_TouchButtons_AddButtonToList( name, texture, command, color, flags );
}

#ifdef __vita__

//===============================
// exports for vita dlsym() shit
//===============================

typedef struct dllexport_s
{
	const char *name;
	void *func;
} dllexport_t;

dllexport_t vita_mainui_exports[] =
{
	{ "GetMenuAPI", (void*)GetMenuAPI },
	{ "GiveTextAPI", (void*)GiveTextAPI },
	{ "AddTouchButtonToList", (void*)AddTouchButtonToList },
	{ NULL, NULL },
};

extern "C"
{

#include <vitasdk.h>

// hacks to make libc work

void* __dso_handle = (void*) &__dso_handle;

extern void _init_vita_reent( void );
extern void _free_vita_reent( void );

extern void __libc_init_array( void );
extern void __libc_fini_array( void );

void _init_vita_newlib( void )
{
	_init_vita_reent( );
}

void _free_vita_newlib( void )
{
	_free_vita_reent( );
}

void _fini( void ) { }
void _init( void ) { }

// just in case
unsigned int _newlib_heap_size_user = 4096;

typedef struct sysfuncs_s
{
	// mem
	void* (*pfnSysMalloc)(size_t);
	void* (*pfnSysCalloc)(size_t, size_t);
	void* (*pfnSysRealloc)(void*, size_t);
	void  (*pfnSysFree)(void*);
	// i/o
	FILE* (*pfnSysFopen)(const char*, const char*);
	int (*pfnSysFclose)(FILE*);
	int (*pfnSysFseek)(FILE*, long int, int);
	long int (*pfnSysFtell)(FILE*);
	int (*pfnSysFprintf)(FILE*, const char*, ...);
	size_t (*pfnSysFread)(void*, size_t, size_t, FILE*);
	size_t (*pfnSysFwrite)(const void*, size_t, size_t, FILE*);
} sysfuncs_t;

typedef struct modarg_s
{
	sysfuncs_t imports;
	dllexport_t *exports;
} modarg_t;

int module_stop( SceSize argc, const void *args )
{
	__libc_fini_array( );
	_free_vita_newlib( );
	return SCE_KERNEL_STOP_SUCCESS;
}

int module_exit( )
{
	__libc_fini_array( );
	_free_vita_newlib( );
	return SCE_KERNEL_STOP_SUCCESS;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start( SceSize argc, void *args )
{
	_init_vita_newlib( );
	__libc_init_array( );

	modarg_t *arg = *(modarg_t **)args;
	arg->exports = vita_mainui_exports;
	return SCE_KERNEL_START_SUCCESS;
}

}

#endif
