#pragma once

#ifndef __VID_VITA_H
#define __VID_VITA_H

#include "../platform/vita/gl_stubs.h"
#include "../platform/vita/gl_mangle.h"

#define MAX_VGL_ARRAYSIZE 1024

// SHADERS
// fragment shaders
#define VGL_SHADER_MODULATE_WITH_COLOR  0
#define VGL_SHADER_MODULATE             1
#define VGL_SHADER_REPLACE              2
#define VGL_SHADER_MONO_COLOR           4
#define VGL_SHADER_MODULATE_COLOR_A     5
#define VGL_SHADER_MODULATE_A           6
#define VGL_SHADER_RGBA_A               7
#define VGL_SHADER_REPLACE_A            8
// vertex shaders
#define VGL_SHADER_TEXTURE2D            0
#define VGL_SHADER_TEXTURE2D_WITH_COLOR 1
#define VGL_SHADER_COLOR                2
#define VGL_SHADER_VERTEX_ONLY          3
// shader programs
#define VGL_SHADER_TEX2D_REPL           0
#define VGL_SHADER_TEX2D_MODUL          1
#define VGL_SHADER_TEX2D_MODUL_CLR      2
#define VGL_SHADER_RGBA_COLOR           3
#define VGL_SHADER_NO_COLOR             4
#define VGL_SHADER_TEX2D_REPL_A         5
#define VGL_SHADER_TEX2D_MODUL_A        6
#define VGL_SHADER_RGBA_CLR_A           7
#define VGL_SHADER_FULL_A               8

extern float *gl_vgl_verts;
extern float *gl_vgl_texcoords;
extern float *gl_vgl_colors;
extern uint16_t *gl_vgl_indices;

void Vita_BeginFrame( void );
void Vita_EndFrame( void );
void Vita_DrawGLPoly( GLenum prim, int num, GLboolean implicit_wvp );
void Vita_EnableGLState( int state );
void Vita_DisableGLState( int state );
void Vita_ReloadShaders( void );
void Vita_FreeShaders( void );

static inline void Vita_VertexPointer( int count, GLenum type, int stride, int num, void *ptr )
{
	vglVertexAttribPointer( 0, count, type, GL_FALSE, 0, num, ptr );
}

static inline void Vita_TexCoordPointer( int count, GLenum type, int stride, int num, void *ptr )
{
	vglVertexAttribPointer( 1, count, type, GL_FALSE, 0, num, ptr );
}

static inline void Vita_ColorPointer( int count, GLenum type, int stride, int num, void *ptr )
{
	vglVertexAttribPointer( 2, count, type, GL_FALSE, 0, num, ptr );
}


#endif
