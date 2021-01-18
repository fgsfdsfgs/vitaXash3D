/*
vid_vita.c - vitagl video backend
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
#if XASH_VIDEO == VIDEO_VITA
#include "client.h"
#include "gl_local.h"
#include "mod_local.h"
#include "input.h"
#include "gl_vidnt.h"

float *gl_vgl_verts = NULL;
float *gl_vgl_texcoords = NULL;
float *gl_vgl_colors = NULL;
uint16_t *gl_vgl_indices = NULL;
vgl_prog_t *gl_vgl_prog = NULL;

typedef enum
{
	rserr_ok,
	rserr_invalid_fullscreen,
	rserr_invalid_mode,
	rserr_unknown
} rserr_t;

extern qboolean vita_keyboard_on;

void Vita_BeginFrame( void )
{
	vglStartRendering( );
}

void Vita_EndFrame( void )
{
	if( vita_keyboard_on )
	{
		vglStopRenderingInit( );
		vglUpdateCommonDialog( );
		vglStopRenderingTerm( );
	}
	else
	{
		vglStopRendering( );
	}
}

/*
=================
GL_GetProcAddress
=================
*/
void *GL_GetProcAddress( const char *name )
{
	return NULL;
}

/*
=================
GL_CheckExtension
=================
*/
void GL_CheckExtension( const char *name, const dllfunc_t *funcs, const char *cvarname, int r_ext )
{
}

/*
===============
GL_UpdateGammaRamp
===============
*/
void GL_UpdateGammaRamp( void )
{
	if( !glConfig.deviceSupportsGamma ) return;

	// GL_BuildGammaTable();
}

/*
===============
GL_UpdateSwapInterval
===============
*/
void GL_UpdateSwapInterval( void )
{
	if( gl_swapInterval->modified )
	{
		gl_swapInterval->modified = false;
		MsgDev( D_INFO, "GL_UpdateSwapInterval: %d\n", gl_swapInterval->integer );
		vglWaitVblankStart( gl_swapInterval->integer );
	}
}


/*
===============
GL_ContextError
===============
*/
static void GL_ContextError( void )
{

}

/*
==================
GL_SetupAttributes
==================
*/
void GL_SetupAttributes()
{

}

void GL_InitExtensions( void )
{
	// initialize gl extensions
	GL_SetExtension( GL_OPENGL_110, true );

	// get our various GL strings
	glConfig.vendor_string = pglGetString( GL_VENDOR );
	glConfig.renderer_string = pglGetString( GL_RENDERER );
	glConfig.version_string = pglGetString( GL_VERSION );
	glConfig.extensions_string = pglGetString( GL_EXTENSIONS );
	MsgDev( D_INFO, "Video: %s\n", glConfig.renderer_string );

	// initalize until base opengl functions loaded

	GL_SetExtension( GL_DRAW_RANGEELEMENTS_EXT, false );
	GL_SetExtension( GL_ENV_COMBINE_EXT, false );
	GL_SetExtension( GL_DOT3_ARB_EXT, false );
	GL_SetExtension( GL_TEXTURE_3D_EXT, false );
	GL_SetExtension( GL_SGIS_MIPMAPS_EXT, false );

	// SCE_GXM_MAX_TEXTURE_UNITS is 16, but 4 is enough
	//GL_SetExtension( GL_ARB_MULTITEXTURE, true ); // TODO: make this shit work
	//glConfig.max_texture_units = 4;
	GL_SetExtension( GL_ARB_MULTITEXTURE, false ); 
	glConfig.max_texture_units = 1;

	// there's no hardware cubemaps
	GL_SetExtension( GL_TEXTURECUBEMAP_EXT, false ); 
	GL_SetExtension( GL_ARB_SEAMLESS_CUBEMAP, false );

	GL_SetExtension( GL_EXT_POINTPARAMETERS, false );
	GL_SetExtension( GL_ARB_TEXTURE_NPOT_EXT, false ); // TODO: check for NPOT support

	GL_SetExtension( GL_TEXTURE_COMPRESSION_EXT, false );
	GL_SetExtension( GL_CUSTOM_VERTEX_ARRAY_EXT, false );
	GL_SetExtension( GL_CLAMPTOEDGE_EXT, true ); // vitaGL supports GL_CLAMP_TO_EDGE
	GL_SetExtension( GL_ANISOTROPY_EXT, false );
	GL_SetExtension( GL_TEXTURE_LODBIAS, false );
	GL_SetExtension( GL_CLAMP_TEXBORDER_EXT, false );
	GL_SetExtension( GL_BLEND_MINMAX_EXT, false );
	GL_SetExtension( GL_BLEND_SUBTRACT_EXT, false );
	GL_SetExtension( GL_SEPARATESTENCIL_EXT, false );
	GL_SetExtension( GL_STENCILTWOSIDE_EXT, false );
	GL_SetExtension( GL_ARB_VERTEX_BUFFER_OBJECT_EXT, false );
	GL_SetExtension( GL_TEXTURE_ENV_ADD_EXT,false  );
	GL_SetExtension( GL_SHADER_OBJECTS_EXT, false );
	GL_SetExtension( GL_SHADER_GLSL100_EXT, false );
	GL_SetExtension( GL_VERTEX_SHADER_EXT, false );
	GL_SetExtension( GL_FRAGMENT_SHADER_EXT, false );
	GL_SetExtension( GL_SHADOW_EXT, false );
	GL_SetExtension( GL_ARB_DEPTH_FLOAT_EXT, false );
	GL_SetExtension( GL_OCCLUSION_QUERIES_EXT, false );
	GL_SetExtension( GL_DEPTH_TEXTURE, false );

	glConfig.texRectangle = glConfig.max_2d_rectangle_size = 0; // no rectangle
	glConfig.max_2d_texture_size = 4096;

	Cvar_Get( "gl_max_texture_size", "0", CVAR_INIT, "opengl texture max dims" );
	Cvar_Set( "gl_max_texture_size", va( "%i", glConfig.max_2d_texture_size ) );

	// SCE_GXM_MAX_UNIFORM_BUFFERS is 8, but whatever
	glConfig.max_vertex_uniforms = 0;
	// SCE_GXM_MAX_VERTEX_ATTRIBUTES is 16, but whatever
	glConfig.max_vertex_attribs = 0;

	glw_state.initialized = true;

	tr.framecount = tr.visframecount = 1;
}


/*
=================
GL_CreateContext
=================
*/
qboolean GL_CreateContext( void )
{
	return true;
}

/*
=================
GL_UpdateContext
=================
*/
qboolean GL_UpdateContext( void )
{
	return true;
}

/*
=================
GL_DeleteContext
=================
*/
qboolean GL_DeleteContext( void )
{
	return false;
}

/*
=================
VID_*
=================
*/

uint VID_EnumerateInstances( void )
{
	return 1;
}

void VID_StartupGamma( void )
{
	// Device supports gamma anyway, but cannot do anything with it.
	fs_offset_t gamma_size;
	byte *savedGamma;
	size_t gammaTypeSize = sizeof( glState.stateRamp );

	// init gamma ramp
	Q_memset( glState.stateRamp, 0, gammaTypeSize );

	// force to set cvar
	Cvar_FullSet( "gl_ignorehwgamma", "1", CVAR_GLCONFIG );

	glConfig.deviceSupportsGamma = false;	// even if supported!
	BuildGammaTable( vid_gamma->value, vid_texgamma->value );
	MsgDev( D_NOTE, "VID_StartupGamma: software gamma initialized\n" );
}

void VID_RestoreGamma( void )
{
}

qboolean VID_CreateWindow( int width, int height, qboolean fullscreen )
{
	return true;
}

void VID_DestroyWindow( void )
{
	if( glState.fullScreen )
	{
		glState.fullScreen = false;
	}
}

/*
==================
R_ChangeDisplaySettingsFast

Change window size fastly to custom values, without setting vid mode
==================
*/
void R_ChangeDisplaySettingsFast( int width, int height )
{
	//Cvar_SetFloat("vid_mode", VID_NOMODE);
	Cvar_SetFloat( "width", width );
	Cvar_SetFloat( "height", height );
	MsgDev( D_NOTE, "R_ChangeDisplaySettingsFast(%d, %d)\n", width, height);

	glState.width = width;
	glState.height = height;
	host.window_center_x = width / 2;
	host.window_center_y = height / 2;

	glState.wideScreen = true; // V_AdjustFov will check for widescreen

	SCR_VidInit();
}


rserr_t R_ChangeDisplaySettings( int width, int height, qboolean fullscreen )
{
	R_SaveVideoMode( width, height );

	host.window_center_x = width / 2;
	host.window_center_y = height / 2;

	glState.wideScreen = true; // V_AdjustFov will check for widescreen

	return rserr_ok;
}


qboolean VID_SetScreenResolution( int width, int height )
{
	return false;
}

void VID_RestoreScreenResolution( void )
{

}

/*
==================
VID_SetMode

Set the described video mode
==================
*/
qboolean VID_SetMode( void )
{
	qboolean	fullscreen = false;
	int iScreenWidth, iScreenHeight;
	rserr_t	err;

	Cvar_SetFloat( "fullscreen", 1 );

	if( vid_mode->modified && vid_mode->integer >= 0 && vid_mode->integer <= num_vidmodes )
	{
		iScreenWidth = vidmode[vid_mode->integer].width;
		iScreenHeight = vidmode[vid_mode->integer].height;
	}
	else
	{
		iScreenWidth = 960;
		iScreenHeight = 544;
	}

	gl_swapInterval->modified = true;
	fullscreen = Cvar_VariableInteger("fullscreen") != 0;

	if(( err = R_ChangeDisplaySettings( iScreenWidth, iScreenHeight, fullscreen )) == rserr_ok )
	{
		glConfig.prev_width = iScreenWidth;
		glConfig.prev_height = iScreenHeight;
	}
	else
	{
		if( err == rserr_invalid_fullscreen )
		{
			Cvar_SetFloat( "fullscreen", 0 );
			MsgDev( D_ERROR, "VID_SetMode: fullscreen unavailable in this mode\n" );
			Sys_Warn("fullscreen unavailable in this mode!");
			if(( err = R_ChangeDisplaySettings( iScreenWidth, iScreenHeight, false )) == rserr_ok )
				return true;
		}
		else if( err == rserr_invalid_mode )
		{
			Cvar_SetFloat( "vid_mode", glConfig.prev_mode );
			MsgDev( D_ERROR, "VID_SetMode: invalid mode\n" );
			Sys_Warn("invalid mode");
		}

		// try setting it back to something safe
		if(( err = R_ChangeDisplaySettings( glConfig.prev_width, glConfig.prev_height, false )) != rserr_ok )
		{
			MsgDev( D_ERROR, "VID_SetMode: could not revert to safe mode\n" );
			Sys_Warn("could not revert to safe mode!");
			return false;
		}
	}

	if( glw_state.initialized )
	{
		// can't change res on the fly, so restart the whole fucking game
		Sys_Restart( );
	}

	return true;
}

/*
==================
R_Init_OpenGL
==================
*/
qboolean R_Init_OpenGL( void )
{
	VID_StartupGamma();
	MsgDev( D_NOTE, "R_Init_OpenGL()\n" );

	gl_vgl_indices = malloc( sizeof( uint16_t ) * MAX_VGL_ARRAYSIZE );
	for( uint16_t i = 0; i < MAX_VGL_ARRAYSIZE; ++i ) gl_vgl_indices[i] = i;

	gl_vgl_verts = calloc( MAX_VGL_ARRAYSIZE * 3, sizeof( GLfloat ) );
	gl_vgl_colors = calloc( MAX_VGL_ARRAYSIZE * 4, sizeof( GLfloat ) );
	gl_vgl_texcoords = calloc( MAX_VGL_ARRAYSIZE * 2, sizeof( GLfloat ) );

	qboolean ret = VID_SetMode();
	printf( "R_Init_OpenGL(): %dx%d\n", glState.width, glState.height );

	vglUseVram( GL_TRUE );
	vglUseExtraMem( GL_FALSE );
	vglInitExtended( 0x800000, glState.width, glState.height, 0x1000000, SCE_GXM_MULTISAMPLE_NONE );
	vglWaitVblankStart( GL_TRUE );

	vglIndexPointerMapped( gl_vgl_indices );

	Vita_ReloadShaders( );
	Vita_EnableGLState( GL_ALPHA_TEST );
	Vita_EnableGLState( GL_TEXTURE_COORD_ARRAY );

	return ret;
}


/*
==================
R_Free_OpenGL
==================
*/
void R_Free_OpenGL( void )
{
	MsgDev( D_NOTE, "R_Free_OpenGL()\n");

	VID_RestoreGamma( );

	GL_DeleteContext( );

	VID_DestroyWindow( );

	Vita_FreeShaders( );

	vglEnd( );

	// now all extensions are disabled
	Q_memset( glConfig.extension, 0, sizeof( glConfig.extension[0] ) * GL_EXTCOUNT );
	glw_state.initialized = false;
}

/* GL WRAPPERS */

// hackfix alpha test bugs and texture format inconsistencies
// backported from FWGS' nanogl
void pglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	uint8_t *in = (uint8_t*)pixels;

	if( internalformat == GL_RGB && format == GL_RGBA ) // strip alpha from texture
	{
		int i = 0, size = width * height * 4;
		for( i = 0; i < size; i += 4, in += 4 )
			in[3] = 255;
	}

	internalformat = format = GL_RGBA;
	glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

/* SHADER STUFF */
// mostly stolen from vitaQuake

static GLuint fs[9];
static GLuint vs[4];

static vgl_prog_t programs[VGL_SHADER_PROG_COUNT] =
{
	// frag                           vert                               pos      texcoord  color
	{ VGL_SHADER_REPLACE,             VGL_SHADER_TEXTURE2D,            { GL_TRUE, GL_TRUE,  GL_FALSE } },
	{ VGL_SHADER_MODULATE,            VGL_SHADER_TEXTURE2D,            { GL_TRUE, GL_TRUE,  GL_FALSE } },
	{ VGL_SHADER_MODULATE_WITH_COLOR, VGL_SHADER_TEXTURE2D_WITH_COLOR, { GL_TRUE, GL_TRUE,  GL_TRUE  } },
	{ VGL_SHADER_RGBA_COLOR,          VGL_SHADER_COLOR,                { GL_TRUE, GL_FALSE, GL_TRUE  } },
	{ VGL_SHADER_MONO_COLOR,          VGL_SHADER_VERTEX_ONLY,          { GL_TRUE, GL_FALSE, GL_FALSE } },
	{ VGL_SHADER_REPLACE_A,           VGL_SHADER_TEXTURE2D,            { GL_TRUE, GL_TRUE,  GL_FALSE } },
	{ VGL_SHADER_MODULATE_A,          VGL_SHADER_TEXTURE2D,            { GL_TRUE, GL_TRUE,  GL_FALSE } },
	{ VGL_SHADER_RGBA_A,              VGL_SHADER_COLOR,                { GL_TRUE, GL_FALSE, GL_TRUE  } },
	{ VGL_SHADER_MODULATE_COLOR_A,    VGL_SHADER_TEXTURE2D_WITH_COLOR, { GL_TRUE, GL_TRUE,  GL_TRUE  } },
};

static qboolean just_color = true; // HACK

static int state_mask = 0;
static int texenv_mask = 0;
static int texcoord_state = 0;
static int alpha_state = 0;
static int color_state = 0;
static float cur_color[4];
static qboolean cur_color_changed = true;
GLint u_monocolor;
GLint u_modcolor[2];

static qboolean shaders_loaded;
static qboolean shaders_reload;

static void LoadShader( const char* filename, GLuint idx, GLboolean fragment )
{
	FILE* f = fopen( filename, "rb" );
	if ( !f )
	{
		Sys_Error( "LoadShader(%s): could not open file\n", filename );
		return;
	}

	fseek( f, 0, SEEK_END );
	long int size = ftell( f );
	fseek( f, 0, SEEK_SET );

	void* res = malloc( size );
	if ( !res )
	{
		fclose( f );
		Sys_Error( "LoadShader(%s): out of memory\n", filename );
		return;
	}

	fread( res, 1, size, f );
	fclose( f );

	if( fragment )
	{
		fs[idx] = glCreateShader( GL_FRAGMENT_SHADER );
		if ( fs[idx] == 0 )
		{
			free( res );
			Sys_Error( "LoadShader(%s): glCreateShader returned %08x\n", glGetError() );
			return;
		}
		glShaderBinary( 1, &fs[idx], 0, res, size );
	}
	else
	{
		vs[idx] = glCreateShader( GL_VERTEX_SHADER );
		if ( vs[idx] == 0 )
		{
			free( res );
			Sys_Error( "LoadShader(%s): glCreateShader returned %08x\n", glGetError() );
			return;
		}
		glShaderBinary( 1, &vs[idx], 0, res, size );
	}

	free( res );
}

static inline void LinkShader( GLuint pidx )
{
	vgl_prog_t *prog = programs + pidx;

	prog->glprog = glCreateProgram( );

	glAttachShader( prog->glprog, fs[prog->fidx] );
	glAttachShader( prog->glprog, vs[prog->vidx] );

	GLuint attrn = 0;
	// position is always used
	prog->attr_idx[VGL_ATTR_POS] = attrn;
	vglBindAttribLocation( prog->glprog, attrn++, "aPosition", 3, GL_FLOAT );
	if( prog->attr_used[VGL_ATTR_TEXCOORD] )
	{
		prog->attr_idx[VGL_ATTR_TEXCOORD] = attrn;
		vglBindAttribLocation( prog->glprog, attrn++, "aTexCoord", 2, GL_FLOAT );
	}
	if( prog->attr_used[VGL_ATTR_COLOR] )
	{
		prog->attr_idx[VGL_ATTR_COLOR] = attrn;
		vglBindAttribLocation( prog->glprog, attrn++, "aColor", 4, GL_FLOAT );
	}

	glLinkProgram( prog->glprog );
}

static inline void UseProgram( GLuint pidx )
{
	gl_vgl_prog = &programs[pidx];
	glUseProgram( gl_vgl_prog->glprog );
}

void Vita_FreeShaders( void )
{
	if( shaders_loaded )
	{
		for( int i = 0; i < 9; i++ )
			glDeleteProgram( programs[i].glprog );
		for( int i = 0; i < 9; i++ )
			glDeleteShader( fs[i] );
		for( int i = 0; i < 4; i++ )
			glDeleteShader( vs[i] );
		shaders_loaded = false;
	}
}

void Vita_ReloadShaders( void )
{
	glFinish( );
	Vita_FreeShaders( );

	LoadShader( "app0:shaders/modulate_f.gxp", VGL_SHADER_MODULATE, GL_TRUE );
	LoadShader( "app0:shaders/modulate_rgba_f.gxp", VGL_SHADER_MODULATE_WITH_COLOR, GL_TRUE );
	LoadShader( "app0:shaders/replace_f.gxp", VGL_SHADER_REPLACE, GL_TRUE );
	LoadShader( "app0:shaders/modulate_alpha_f.gxp", VGL_SHADER_MODULATE_A, GL_TRUE );
	LoadShader( "app0:shaders/modulate_rgba_alpha_f.gxp", VGL_SHADER_MODULATE_COLOR_A, GL_TRUE );
	LoadShader( "app0:shaders/replace_alpha_f.gxp", VGL_SHADER_REPLACE_A, GL_TRUE );
	LoadShader( "app0:shaders/texture2d_v.gxp", VGL_SHADER_TEXTURE2D, GL_FALSE );
	LoadShader( "app0:shaders/texture2d_rgba_v.gxp", VGL_SHADER_TEXTURE2D_WITH_COLOR, GL_FALSE );
	LoadShader( "app0:shaders/rgba_f.gxp", VGL_SHADER_RGBA_COLOR, GL_TRUE );
	LoadShader( "app0:shaders/vertex_f.gxp", VGL_SHADER_MONO_COLOR, GL_TRUE );
	LoadShader( "app0:shaders/rgba_alpha_f.gxp", VGL_SHADER_RGBA_A, GL_TRUE );
	LoadShader( "app0:shaders/rgba_v.gxp", VGL_SHADER_COLOR, GL_FALSE );
	LoadShader( "app0:shaders/vertex_v.gxp", VGL_SHADER_VERTEX_ONLY, GL_FALSE );

	for ( GLuint i = 0; i < VGL_SHADER_PROG_COUNT; ++i)
		LinkShader( i );

	u_modcolor[0] = glGetUniformLocation( programs[VGL_SHADER_TEX2D_MODUL].glprog, "uColor" );
	u_modcolor[1] = glGetUniformLocation( programs[VGL_SHADER_TEX2D_MODUL_A].glprog, "uColor" );
	u_monocolor = glGetUniformLocation( programs[VGL_SHADER_NO_COLOR].glprog, "uColor" );
}

void Vita_SetShader( void )
{
	just_color = false;
	switch( state_mask + texenv_mask )
	{
		case 0x00: // Everything off
		case 0x04: // Modulate
		case 0x08: // Alpha Test
		case 0x0C: // Alpha Test + Modulate
			just_color = true;
			UseProgram( VGL_SHADER_NO_COLOR );
			break;
		case 0x01: // Texcoord
		case 0x03: // Texcoord + Color
			UseProgram( VGL_SHADER_TEX2D_REPL );
			break;
		case 0x02: // Color
		case 0x06: // Color + Modulate
			UseProgram( VGL_SHADER_RGBA_COLOR );
			break;
		case 0x05: // Modulate + Texcoord
			UseProgram( VGL_SHADER_TEX2D_MODUL );
			break;
		case 0x07: // Modulate + Texcoord + Color
			UseProgram( VGL_SHADER_TEX2D_MODUL_CLR );
			break;
		case 0x09: // Alpha Test + Texcoord
		case 0x0B: // Alpha Test + Color + Texcoord
			UseProgram( VGL_SHADER_TEX2D_REPL_A );
			break;
		case 0x0A: // Alpha Test + Color
		case 0x0E: // Alpha Test + Modulate + Color
			UseProgram( VGL_SHADER_RGBA_CLR_A );
			break;
		case 0x0D: // Alpha Test + Modulate + Texcoord
			UseProgram( VGL_SHADER_TEX2D_MODUL_A );
			break;
		case 0x0F: // Alpha Test + Modulate + Texcoord + Color
			UseProgram( VGL_SHADER_FULL_A );
			break;
		default:
			break;
	}
}

void Vita_EnableGLState( int state )
{
	switch( state )
	{
		case GL_TEXTURE_COORD_ARRAY:
			if( !texcoord_state )
			{
				state_mask += 0x01;
				texcoord_state = 1;
			}
			break;
		case GL_COLOR_ARRAY:
			if( !color_state )
			{
				state_mask += 0x02;
				color_state = 1;
			}
			break;
		case GL_MODULATE:
			texenv_mask = 0x04;
			break;
		case GL_REPLACE:
			texenv_mask = 0;
			break;
		case GL_ALPHA_TEST:
			if( !alpha_state )
			{
				state_mask += 0x08;
				alpha_state = 1;
			} 
			break;
		default: return;
	}
	Vita_SetShader( );
}

void Vita_DisableGLState( int state )
{
	switch( state )
	{
		case GL_TEXTURE_COORD_ARRAY:
			if( texcoord_state )
			{
				state_mask -= 0x01;
				texcoord_state = 0;
			}
			break;
		case GL_COLOR_ARRAY:
			if( color_state )
			{
				state_mask -= 0x02;
				color_state = 0;
			}
			break;
		case GL_ALPHA_TEST:
			if( alpha_state )
			{
				state_mask -= 0x08;
				alpha_state = 0;
			} 
			break;
		default: return;
	}
	Vita_SetShader( );
}

void Vita_DrawGLPoly( GLenum prim, int num, GLboolean implicit_wvp )
{
	if( cur_color_changed )
	{
		if( (state_mask + texenv_mask) == 0x05 ) glUniform4fv( u_modcolor[0], 1, cur_color );
		else if( (state_mask + texenv_mask) == 0x0D ) glUniform4fv( u_modcolor[1], 1, cur_color );
		else if( just_color ) glUniform4fv( u_monocolor, 1, cur_color );
		cur_color_changed = false;
	}
	vglDrawObjects( prim, num, implicit_wvp );
}

// HACKHACKHACK: GL function wrappers to go with this shit

void pglColor3f( float r, float g, float b )
{
	cur_color[0] = r;
	cur_color[1] = g;
	cur_color[2] = b;
	cur_color[3] = 1.f;
	cur_color_changed = true;
}

void pglColor4f( float r, float g, float b, float a )
{
	cur_color[0] = r;
	cur_color[1] = g;
	cur_color[2] = b;
	cur_color[3] = a;
	cur_color_changed = true;
}

void pglColor4ub( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
	cur_color[0] = r / 255.f;
	cur_color[1] = g / 255.f;
	cur_color[2] = b / 255.f;
	cur_color[3] = a / 255.f;
	cur_color_changed = true;
}

void pglColor4ubv( uint8_t *v )
{
	cur_color[0] = *(v++) / 255.f;
	cur_color[1] = *(v++) / 255.f;
	cur_color[2] = *(v++) / 255.f;
	cur_color[3] = *(v  ) / 255.f;
	cur_color_changed = true;
}

void pglTexEnvi( GLenum target, GLenum pname, GLint param )
{
	if( param == GL_MODULATE || param == GL_REPLACE ) Vita_EnableGLState( param );
	else glTexEnvi( target, pname, param );
}

void pglEnable( GLenum param )
{
	if( param == GL_ALPHA_TEST || param == GL_TEXTURE_2D ) Vita_EnableGLState( param );
	else glEnable( param );
}

void pglDisable( GLenum param )
{
	if( param == GL_ALPHA_TEST || param == GL_TEXTURE_2D ) Vita_DisableGLState( param );
	else glDisable( param );
}

void pglEnableClientState( GLenum param )
{
	if( param == GL_COLOR_ARRAY || param == GL_TEXTURE_COORD_ARRAY || param == GL_VERTEX_ARRAY ) Vita_EnableGLState( param );
	else glEnableClientState( param );
}

void pglDisableClientState( GLenum param )
{
	if( param == GL_COLOR_ARRAY || param == GL_TEXTURE_COORD_ARRAY || param == GL_VERTEX_ARRAY ) Vita_DisableGLState( param );
	else glDisableClientState( param );
}

#endif // XASH_VIDEO
