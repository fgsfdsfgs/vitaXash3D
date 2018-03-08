#ifndef DLL_VITA_H
#define DLL_VITA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define RTLD_LAZY 0x0001
#define RTLD_NOW  0x0002

typedef struct dllexport_s
{
	const char *name;
	void *func;
} dllexport_t;

typedef struct Dl_info_s
{
	void *dli_fhandle;
	const char *dli_sname;
	const void *dli_saddr;
} Dl_info;

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

void *dlsym(void *handle, const char *symbol );
void *dlopen(const char *name, int flag );
int dlclose(void *handle);
char *dlerror( void );
int dladdr( const void *addr, Dl_info *info );

#ifdef __cplusplus
}
#endif

#endif
