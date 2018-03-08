/*
dll_vita.c - libdl function wrappers for vita
Copyright (C) 2018 fgsfds

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"
#include "dll_vita.h"
#include <vitasdk.h>

#define MAX_DLNAMELEN 256

typedef struct dll_s
{
	SceUID handle;
	char name[MAX_DLNAMELEN];
	int refcnt;
	dllexport_t *exp;
	struct dll_s *next;
} dll_t;

typedef struct modarg_s
{
	sysfuncs_t imports;
	dllexport_t *exports;
} modarg_t;

static sysfuncs_t sys_exports = {
	// mem
	malloc,
	calloc,
	realloc,
	free,
	// io
	fopen,
	fclose,
	fseek,
	ftell,
	fprintf,
	fread,
	fwrite,
};

static modarg_t modarg;

static dll_t *dll_list;
static char *dll_err = NULL;
static char dll_err_buf[1024];

static void *dlfind( const char *name )
{
	dll_t *d = NULL;
	for( d = dll_list; d; d = d->next )
		if( !Q_strcmp( d->name, name ) )
			break;
	return d;
}

static const char *dlname( void *handle )
{
	dll_t *d = NULL;
	// iterate through all dll_ts to check if the handle is actually in the list
	// and not some bogus pointer from god knows where
	for( d = dll_list; d; d = d->next ) if( d == handle ) break;
	return d ? d->name : NULL;
}

void *dlopen( const char *name, int flag )
{
	if( !name ) return NULL;

	dll_t *old = dlfind( name );
	if( old ) { old->refcnt++; return old; }

	char fullpath[MAX_SYSPATH];
	Q_snprintf( fullpath, sizeof(fullpath), CWD "/%s", name );

	modarg.imports = sys_exports;
	modarg.exports = NULL;

	int status = 0;
	modarg_t *arg = &modarg;
	SceUID h = sceKernelLoadStartModule( fullpath, sizeof( arg ), &arg, 0, NULL, &status );

	if( !h ) { dll_err = "dlopen(): something went wrong"; return NULL; }
	if( h < 0 )
	{
		snprintf( dll_err_buf, sizeof( dll_err_buf ), "dlopen(%s): error 0x%X\n", name, h );
		dll_err = dll_err_buf;
		return NULL;
	}

	if( status == SCE_KERNEL_START_FAILED || status == SCE_KERNEL_START_NO_RESIDENT )
	{
		dll_err = "dlopen(): module_start() failed";
		return NULL;
	}

	if( !modarg.exports )
	{
		dll_err = "dlopen(): NULL exports";
		return NULL;
	}

	dll_t *new = calloc( 1, sizeof( dll_t ) );
	if( !new ) { dll_err = "dlopen(): out of memory";  return NULL; }
	snprintf( new->name, MAX_DLNAMELEN, name );
	new->handle = h;
	new->exp = modarg.exports;
	new->refcnt = 1;

	new->next = dll_list;
	dll_list = new;

	return new;
}

void *dlsym( void *handle, const char *symbol )
{
	if( !handle || !symbol ) { dll_err = "dlsym(): NULL args"; return NULL; }
	if( !dlname( handle ) ) { dll_err = "dlsym(): unknown handle"; return NULL; }
	dll_t *d = handle;
	if( !d->refcnt ) { dll_err = "dlsym(): call dlopen() first"; return NULL; }
	dllexport_t *f = NULL;
	for( f = d->exp; f && f->func; f++ )
		if( !Q_strcmp( f->name, symbol ) )
			break;

	if( f && f->func )
	{
		return f->func;
	}
	else
	{
		dll_err = "dlsym(): symbol not found in dll";
		return NULL;
	}
}

int dlclose( void *handle )
{
	if( !handle ) { dll_err = "dlclose(): NULL arg"; return -1; }
	if( !dlname( handle ) ) { dll_err = "dlclose(): unknown handle"; return -2; }

	dll_t *d = handle;
	d->refcnt--;
	if( d->refcnt <= 0 )
	{
		int status = 0;
		int ret = sceKernelStopUnloadModule( d->handle, 0, NULL, 0, NULL, &status );
		if( ret != SCE_OK )
		{
			snprintf( dll_err_buf, sizeof( dll_err_buf ), "dlclose(): error %d", ret );
			dll_err = dll_err_buf;
		}
		else if( status == SCE_KERNEL_STOP_CANCEL )
		{
			dll_err = "dlclose(): module doesn't want to stop";
			return -3;
		}

		if( d == dll_list )
			dll_list = NULL;
		else
			for( dll_t *pd = dll_list; pd; pd = pd->next )
			{
				if( pd->next == d )
				{
					pd->next = d->next;
					break;
				}
			}

		free( d );
	}

	return 0;
}

char *dlerror( void )
{
	char *err = dll_err;
	dll_err = NULL;
	return err;
}

int dladdr( const void *addr, Dl_info *info )
{
	dll_t *d = NULL;
	dllexport_t *f = NULL;
	for( d = dll_list; d; d = d->next )
		for( f = d->exp; f && f->func; f++ )
			if( f->func == addr ) goto for_end;
for_end:
	if( d && f && f->func )
	{
		if( info )
		{
			info->dli_fhandle = d;
			info->dli_sname = f->name;
			info->dli_saddr = addr;
		}
		return 1;
	}
	return 0;
}
