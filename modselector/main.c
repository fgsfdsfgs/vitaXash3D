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
	const char *dll;
} mods[MAXMODS];

static struct mod_s *mod_order[MAXMODS];
static int nummods = 0;

/*---------------------------------------------------------------------------*/

static int mod_get_info( struct mod_s *mod, char *dir )
{
	char path[STRSIZE];
	char *data = NULL;

	mod->title[0] = 0;
	mod->dll = "???";

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

	snprintf( path, sizeof(path), CWD "/%s/dlls/server.suprx", dir );
	int sv_dll = fexists( path );
	snprintf( path, sizeof(path), CWD "/%s/cl_dlls/client.suprx", dir );
	int cl_dll = fexists( path );

	if( sv_dll && cl_dll )
		mod->dll = "Yes";
	else if( sv_dll )
		mod->dll = "Server";
	else if( cl_dll )
		mod->dll = "Client";
	else
		mod->dll = "No";

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
		if( !ep->d_name || ep->d_name[0] == '.' || !strcmp( ep->d_name, "launcher" ) )
			continue;
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

static vita2d_texture *img_bg;
static vita2d_pgf *pgf;

static vita2d_texture *fnt_small;
static const int fnt_small_sz = 12;
static const int fnt_small_cw[] =
{
#include "charwidths.h"
};

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
const unsigned int C_DKGREY = RGBA8( 80, 80, 80, 255 );
const unsigned int C_BLACK = RGBA8( 0, 0, 0, 255 );
const unsigned int C_GREEN = RGBA8( 0, 255, 0, 255 );
const unsigned int C_YELLOW = RGBA8( 255, 255, 0, 255 );
const unsigned int C_ORANGE = RGBA8( 255, 180, 0, 255 );
const unsigned int C_BROWN = RGBA8( 80, 64, 25, 200 );

static void gfx_printf( int flags, int x, int y, unsigned int c, float s, const char *fmt, ... )
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

// NOTE: does not support \n
static void gfx_printf_small( int flags, int x, int y, unsigned int col, const char *fmt, ... )
{
	static char buf[4096];

	va_list argptr;
	va_start( argptr, fmt );
	vsnprintf( buf, sizeof(buf), fmt, argptr );
	va_end( argptr );

	if( !fnt_small ) return;

	// don't care about performance, it's a fucking menu
	int txtw = 0, txth = fnt_small_sz;
	for( char *c = buf; *c && c < buf + 4096; ++c )
		txtw += fnt_small_cw[(uint8_t)(*c)];

	if( flags & P_XCENTER )
		x -= txtw / 2;
	else if( flags & P_ARIGHT )
		x -= txtw;

	if( flags & P_YCENTER )
		y -= txth / 2;
	else if( flags & P_ABOTTOM )
		y -= txth;

	for( char *c = buf; *c && c < buf + 4096; ++c )
	{
		int cw = fnt_small_cw[(uint8_t)(*c)];
		int tx = fnt_small_sz * ((*c) % 16);
		int ty = fnt_small_sz * ((*c) / 16);
		vita2d_draw_texture_tint_part( fnt_small, x, y, tx, ty, cw, fnt_small_sz, col );
		x += cw;
	}
}

static void gfx_box( int x, int y, int to_x, int to_y, int w, unsigned int c )
{
	vita2d_draw_rectangle( x, y, to_x - x, w, c );
	vita2d_draw_rectangle( x, to_y - w, to_x - x, w, c );
	vita2d_draw_rectangle( x, y, w, to_y - y, c );
	vita2d_draw_rectangle( to_x - w, y, w, to_y - y, c );
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

static inline void cleanup( void )
{
	vita2d_fini( );

	if( pgf ) vita2d_free_pgf( pgf );
	if( img_bg ) vita2d_free_texture( img_bg );
	if( fnt_small ) vita2d_free_texture( fnt_small );

	pgf = NULL;
	img_bg = NULL;
	fnt_small = NULL;
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

	cleanup( );
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
		cleanup( );
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
		cleanup( );
		return 1;
	}

	return !b_held( SCE_CTRL_START );
}

static void draw( void )
{
	if( img_bg ) vita2d_draw_texture( img_bg, 0.f, 0.f );

	gfx_printf_small( P_ARIGHT | P_ABOTTOM, 960 - 4, 544 - 4, C_ORANGE, "vitaXash3D %s/%s, build date %s %s",
		XASH_VERSION, XASH_BUILD_COMMIT, __DATE__, __TIME__ );

	gfx_printf( P_XCENTER, CX, 544 - 68, C_ORANGE, 1.f, "[^] [v] Select    [x] Run    [\\] Debug mode    [start] Exit" );

	// game table //
	gfx_box( 60, 160, 960 - 60, 544 - 96, 3, C_DKGREY );
	// header
	gfx_printf( 0, 72 + 0, 152, C_LTGREY, 1.f, "Directory" );
	gfx_printf( 0, 72 + 160, 152, C_LTGREY, 1.f, "DLLs" );
	gfx_printf( 0, 72 + 256, 152, C_LTGREY, 1.f, "Name" );
	for( int i = 0; i < nummods; ++i )
	{
		int selected = ( menu_sel == i );
		unsigned int color = selected ? C_YELLOW : C_ORANGE;
		if( selected ) vita2d_draw_rectangle( 60 + 4, 160 + 4 + i*20, 960 - 60 - 64 - 4, 19, C_BROWN );
		gfx_printf( P_YCENTER, 72 + 0, 160 + 28 + i*20, color, 1.f, mod_order[i]->dir );
		gfx_printf( P_YCENTER, 72 + 160, 160 + 28 + i*20, color, 1.f, mod_order[i]->dll );
		gfx_printf( P_YCENTER, 72 + 256, 160 + 28 + i*20, color, 1.f, mod_order[i]->title );
	}
}

int main( void )
{
	vita2d_init( );
	vita2d_set_clear_color( C_BLACK );
	pgf = vita2d_load_default_pgf( );
	img_bg = vita2d_load_PNG_file( CWD "/launcher/bg.png" );
	fnt_small = vita2d_load_PNG_file( CWD "/launcher/fnt_small.png" );

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

	cleanup( );
	sceKernelExitProcess( 0 );
	return 0;
}
