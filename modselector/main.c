#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/types.h>
#include <psp2/ctrl.h>
#include <vita2d.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include "utils.h"

/*---------------------------------------------------------------------------*/

#define MAXMODS 256
#define STRSIZE 256
#define MAXARGS 16

#define CWD     "ux0:/data/xash3d"
#define BASEDIR "valve"
#define XASH    "app0:/xash.bin"

#define XASH_VERSION "0.19"
#ifndef XASH_BUILD_COMMIT
#define XASH_BUILD_COMMIT "notset"
#endif

/*---------------------------------------------------------------------------*/

static struct mod_s {
	char dir[STRSIZE];
	char title[STRSIZE];
} mods[MAXMODS];

static struct mod_s *mod_order[MAXMODS];
static int nummods = 0;

/*---------------------------------------------------------------------------*/

static int mod_get_info( struct mod_s *mod, char *dir )
{
	char path[STRSIZE];
	char *data = NULL;

	mod->title[0] = 0;

	snprintf( path, sizeof(path), CWD "/%s/gameinfo.txt", dir );
	data = fload( path, NULL );
	if( data ) get_info_key( data, "title", mod->title, STRSIZE ), free( data );

	if( !mod->title[0] )
	{
		snprintf( path, sizeof(path), CWD "/%s/liblist.gam", dir );
		data = fload( path, NULL );
		if( data ) get_info_key( data, "game", mod->title, STRSIZE ), free( data );
	}

	if( !mod->title[0] ) return 0;

	strncpy( mod->dir, dir, STRSIZE );
	return 1;
}

static int mod_scan( void )
{
	static char path[STRSIZE];
	DIR *dp = opendir( CWD );
	if( !dp ) return 0;
	struct dirent *ep;
	while( ep = readdir( dp ) )
	{
		if( !ep->d_name || ep->d_name[0] == '.' ) continue;
		snprintf( path, sizeof(path), CWD "/%s", ep->d_name );
		if( isdir( path ) )
			nummods += mod_get_info( mods + nummods, ep->d_name );
	}
	closedir( dp );

	for( int i = 0; i < nummods; ++i )
		mod_order[i] = mods + i;

	return 1;
}

static int mod_cmp_f( const void *a, const void *b )
{
	const struct mod_s *ma = *(const struct mod_s **)a;
	const struct mod_s *mb = *(const struct mod_s **)b;
	// half-life is always first
	if( !strncmp( ma->dir, "valve", STRSIZE ) ) return -1;
	if( !strncmp( mb->dir, "valve", STRSIZE ) ) return 1;
	// sort the rest by title alphabetically
	return strncmp( ma->title, mb->title, STRSIZE );
}

static void mod_sort( void )
{
	qsort( mod_order, nummods, sizeof(mod_order[0]), mod_cmp_f );
}

/*---------------------------------------------------------------------------*/

static vita2d_pgf *pgf;
static SceCtrlData pad, pad_old;

#define W 960
#define H 544
#define CX (W/2)
#define CY (H/2)

#define P_XCENTER 0x01
#define P_YCENTER 0x02
#define P_ARIGHT  0x04
#define P_ABOTTOM 0x08

const unsigned int C_RED = RGBA8( 255, 0, 0, 255 );
const unsigned int C_WHITE = RGBA8( 255, 255, 255, 255 );
const unsigned int C_LTGREY = RGBA8( 192, 192, 192, 255 );
const unsigned int C_GREY = RGBA8( 128, 128, 128, 255 );
const unsigned int C_DKGREY = RGBA8( 64, 64, 64, 255 );
const unsigned int C_BLACK = RGBA8( 0, 0, 0, 255 );
const unsigned int C_GREEN = RGBA8( 0, 255, 0, 255 );

static int gfx_printf( int flags, int x, int y, unsigned int c, float s, const char *fmt, ... )
{
	static char buf[4096];

	va_list argptr;
	va_start( argptr, fmt );
	vsnprintf( buf, sizeof(buf), fmt, argptr );
	va_end( argptr );

	// don't care about performance, it's a fucking menu
	if( flags & P_XCENTER )
		x -= vita2d_pgf_text_width( pgf, s, buf ) / 2;
	else if( flags & P_ARIGHT )
		x -= vita2d_pgf_text_width( pgf, s, buf );

	if( flags & P_YCENTER )
		y -= vita2d_pgf_text_height( pgf, s, buf ) / 2;
	else if( flags & P_ABOTTOM )
		y -= vita2d_pgf_text_height( pgf, s, buf );

	vita2d_pgf_draw_text( pgf, x, y, c, s, buf );
}

static inline int b_pressed( int b )
{
	return (pad.buttons & b) && !(pad_old.buttons & b);
}

static inline int b_held( int b )
{
	return pad.buttons & b;
}

static inline void b_wait( int button )
{
	while( 1 )
	{
		sceCtrlPeekBufferPositive( 0, &pad, 1 );
		if( b_held( button ) )
			break;
	}
}

static int die( const char *fmt, ... )
{
	static char buf[4096];

	va_list argptr;
	va_start( argptr, fmt );
	vsnprintf( buf, sizeof(buf), fmt, argptr );
	va_end( argptr );

	int t1w = vita2d_pgf_text_width( pgf, 2.f, "FATAL ERROR" );
	int t2w = vita2d_pgf_text_width( pgf, 1.f, buf );
	int t3w = vita2d_pgf_text_width( pgf, 1.f, "Press START to exit" );
	int t3h = vita2d_pgf_text_height( pgf, 1.f, "Press START to exit" );

	vita2d_start_drawing( );
	vita2d_clear_screen( );
	gfx_printf( P_XCENTER, CX, CY-64, C_RED, 2.f, "FATAL ERROR" );
	gfx_printf( P_XCENTER, CX, CY, C_WHITE, 1.f, buf );
	gfx_printf( P_XCENTER, CX, H-t3h-32, C_LTGREY, 1.f, "Press START to exit" );
	vita2d_end_drawing( );
	vita2d_swap_buffers( );
	vita2d_wait_rendering_done( );

	b_wait( SCE_CTRL_START );

	vita2d_fini( );
	sceKernelExitProcess( 0 );
}

/*---------------------------------------------------------------------------*/

static int menu_sel = 0;
static char *xash_argv[MAXARGS];
static int xash_argc = 0;

static int update( void )
{
	if( b_pressed( SCE_CTRL_DOWN ) ) menu_sel++;
	if( b_pressed( SCE_CTRL_UP ) ) menu_sel--;
	if( menu_sel < 0 ) menu_sel = nummods - 1;
	else if( menu_sel >= nummods ) menu_sel = 0;

	if( b_pressed( SCE_CTRL_CROSS ) )
	{
		xash_argv[xash_argc++] = "-game";
		xash_argv[xash_argc++] = mod_order[menu_sel]->dir;
		xash_argv[xash_argc] = NULL;
		vita2d_fini( );
		sceAppMgrLoadExec( XASH, xash_argv, NULL );
		return 1;
	}

	if( b_pressed( SCE_CTRL_TRIANGLE ) )
	{
		xash_argv[xash_argc++] = "-log";
		xash_argv[xash_argc++] = "-dev";
		xash_argv[xash_argc++] = "5";
		xash_argv[xash_argc++] = "-game";
		xash_argv[xash_argc++] = mod_order[menu_sel]->dir;
		xash_argv[xash_argc] = NULL;
		int ret = 0;
		if( ret = sceAppMgrLoadExec( XASH, xash_argv, NULL ) )
			die( "sceAppMgrLoadExec( \"%s\" ): %d", XASH, ret );
		vita2d_fini( );
		return 1;
	}

	return !b_held( SCE_CTRL_START );
}

static void draw( void )
{
	gfx_printf( P_XCENTER, CX, 32, C_WHITE, 2.f, "vitaXash3D Launcher" );
	gfx_printf( P_XCENTER, CX, 72, C_LTGREY, 1.f, "ver. %s/%s, build date %s %s",
		XASH_VERSION, XASH_BUILD_COMMIT, __DATE__, __TIME__ );

	gfx_printf( 0, 64, 128, C_WHITE, 1.f, "Available mods:" );
	for( int i = 0; i < nummods; ++i )
	{
		unsigned int color = ( menu_sel == i ) ? C_GREEN : C_LTGREY;
		gfx_printf( 0, 72, 128 + 32 + i*16, color, 1.f, mod_order[i]->title );
	}

	gfx_printf(
		P_ABOTTOM | P_XCENTER, CX, H-4, C_GREY, 1.f,
		"[UP], [DOWN] choose mod\n"
		"[CROSS] launch mod\n"
		"[TRIANGLE] launch mod in debug mode\n"
		"[START] exit"
	);
}

int main( void )
{
	vita2d_init( );
	vita2d_set_clear_color( C_BLACK );
	pgf = vita2d_load_default_pgf( );

	if( !mod_scan( ) )
		die( "Could not scan directory \"" CWD "\":\n%s", strerror( errno ) );

	if( !nummods )
		die( "No mods found!\n" );

	mod_sort( );
	// after sorting HL is always first, so we can check if it's present
	if( strncmp( mod_order[0]->dir, "valve", STRSIZE ) )
		die( "`valve` directory not present!" );

	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	int state = 1;
	while( state )
	{
		pad_old = pad;
		sceCtrlPeekBufferPositive( 0, &pad, 1 );
		state = update( );
		vita2d_start_drawing( );
		vita2d_clear_screen( );
		draw( );
		vita2d_end_drawing( );
		vita2d_swap_buffers( );
	}

	vita2d_fini( );
	return 0;
}
