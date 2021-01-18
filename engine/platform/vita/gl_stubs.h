/*
gl_stubs.h - opengl func stubs and defs for shit that's not supported by vitaGL
keep an eye on this in case these get implemented in vitaGL for real
 */

#ifndef GL_STUBS_H
#define GL_STUBS_H

#include "common.h"
#include <vitaGL.h>

#define GL_NONE 0

#define GL_FOG 0x0B60
#define GL_FOG_MODE 0x0B65
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_COLOR 0x0B66
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_HINT 0x0C54

#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63

#define GL_LINEAR 0x2601
#define GL_EXP 0x0800
#define GL_NICEST 0x1102

#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_WRAP_R (GL_TEXTURE_WRAP_T+1)
#define GL_CLAMP GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_BORDER GL_CLAMP_TO_EDGE
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON GL_TRIANGLE_FAN

#define GL_ADD_SIGNED GL_ADD

#define GL_TEXTURE_BORDER_COLOR 0x1004

#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01

#define GL_S 0x2000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003

#define GL_CLIP_PLANE0 0x3000

#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63

#define GL_PACK_ALIGNMENT 0x0D05
#define GL_UNPACK_ALIGNMENT 0x0CF5

#define GL_TEXTURE0_ARB GL_TEXTURE0
#define GL_TEXTURE1_ARB GL_TEXTURE1

// fucking formats and extensions and shit
// copied from gl_export.h because I'm a lazy fuck
#define GL_TEXTURE_RECTANGLE_EXT           0x84F5
#define GL_TEXTURE_COMPARE_MODE_ARB        0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB        0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB        0x884E
#define GL_TEXTURE_CUBE_MAP_ARB            0x8513
#define GL_BGRA                            0x80E1
#define GL_TEXTURE_1D                      0x0DE0
#define GL_TEXTURE_3D                      0x806F
#define GL_DEPTH_TEXTURE_MODE_ARB          0x884B
#define GL_COMPRESSED_ALPHA_ARB            0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB        0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB  0x84EB
#define GL_COMPRESSED_INTENSITY_ARB        0x84EC
#define GL_COMPRESSED_RGB_ARB              0x84ED
#define GL_COMPRESSED_RGBA_ARB             0x84EE
#define GL_RGBA32F_ARB                     0x8814
#define GL_RGB32F_ARB                      0x8815
#define GL_ALPHA32F_ARB                    0x8816
#define GL_INTENSITY32F_ARB                0x8817
#define GL_LUMINANCE32F_ARB                0x8818
#define GL_LUMINANCE_ALPHA32F_ARB          0x8819
#define GL_RGBA16F_ARB                     0x881A
#define GL_RGB16F_ARB                      0x881B
#define GL_ALPHA16F_ARB                    0x881C
#define GL_INTENSITY16F_ARB                0x881D
#define GL_LUMINANCE16F_ARB                0x881E
#define GL_LUMINANCE_ALPHA16F_ARB          0x881F
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT    0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT   0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT   0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT   0x83F3
#define GL_DEPTH_COMPONENT16               0x81A5
#define GL_DEPTH_COMPONENT24               0x81A6
#define GL_DEPTH_COMPONENT32               0x81A7
#define GL_DEPTH_COMPONENT32F              0x8CAC
#define GL_TEXTURE_MAX_ANISOTROPY_EXT      0x84FE
#define GL_TEXTURE_LOD_BIAS_EXT            0x8501
#define GL_ALPHA4                          0x803B
#define GL_ALPHA8                          0x803C
#define GL_ALPHA12                         0x803D
#define GL_ALPHA16                         0x803E
#define GL_LUMINANCE4                      0x803F
#define GL_LUMINANCE8                      0x8040
#define GL_LUMINANCE12                     0x8041
#define GL_LUMINANCE16                     0x8042
#define GL_LUMINANCE4_ALPHA4               0x8043
#define GL_LUMINANCE6_ALPHA2               0x8044
#define GL_LUMINANCE8_ALPHA8               0x8045
#define GL_LUMINANCE12_ALPHA4              0x8046
#define GL_LUMINANCE12_ALPHA12             0x8047
#define GL_LUMINANCE16_ALPHA16             0x8048
#define GL_LUMINANCE                       0x1909
#define GL_LUMINANCE_ALPHA                 0x190A
#define GL_DEPTH_COMPONENT                 0x1902
#define GL_INTENSITY                       0x8049
#define GL_INTENSITY4                      0x804A
#define GL_INTENSITY8                      0x804B
#define GL_INTENSITY12                     0x804C
#define GL_INTENSITY16                     0x804D
#define GL_R3_G3_B2                        0x2A10
#define GL_RGB4                            0x804F
#define GL_RGB5                            0x8050
#define GL_RGB8                            0x8051
#define GL_RGB10                           0x8052
#define GL_RGB12                           0x8053
#define GL_RGB16                           0x8054
#define GL_RGBA2                           0x8055
#define GL_RGBA4                           0x8056
#define GL_RGB5_A1                         0x8057
#define GL_RGBA8                           0x8058
#define GL_RGB10_A2                        0x8059
#define GL_RGBA12                          0x805A
#define GL_RGBA16                          0x805B
#define GL_GENERATE_MIPMAP_SGIS            0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS       0x8192
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_COMBINE_ARB                     0x8570
#define GL_COMBINE_RGB_ARB                 0x8571
#define GL_COMBINE_ALPHA_ARB               0x8572
#define GL_SOURCE0_RGB_ARB                 0x8580
#define GL_SOURCE1_RGB_ARB                 0x8581
#define GL_SOURCE2_RGB_ARB                 0x8582
#define GL_SOURCE0_ALPHA_ARB               0x8588
#define GL_SOURCE1_ALPHA_ARB               0x8589
#define GL_SOURCE2_ALPHA_ARB               0x858A
#define GL_OPERAND0_RGB_ARB                0x8590
#define GL_OPERAND1_RGB_ARB                0x8591
#define GL_OPERAND2_RGB_ARB                0x8592
#define GL_OPERAND0_ALPHA_ARB              0x8598
#define GL_OPERAND1_ALPHA_ARB              0x8599
#define GL_OPERAND2_ALPHA_ARB              0x859A
#define GL_RGB_SCALE_ARB                   0x8573
#define GL_ADD_SIGNED_ARB                  0x8574
#define GL_INTERPOLATE_ARB                 0x8575
#define GL_SUBTRACT_ARB                    0x84E7
#define GL_CONSTANT_ARB                    0x8576
#define GL_PRIMARY_COLOR_ARB               0x8577
#define GL_PREVIOUS_ARB                    0x8578
#define GL_TEXTURE                         0x1702

static inline void glShadeModel( GLenum model ) { }
static inline void glTexGeni( GLenum coord, GLenum pname, GLint param ) { }
static inline void glPixelStorei( GLenum opt, GLint val ) { }
static inline GLboolean glIsTexture( GLenum obj ) { return obj >= 0 && obj < 2000; }
static inline void glNormal3fv( GLfloat *v ) { }

static inline void glTexParameterfv( GLenum targ, GLenum opt, GLfloat *params ) { glTexParameterf( targ, opt, params[0] ); }

// wrappers, defined in vid_vita.c
void pglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
void pglColor3f( float r, float g, float b );
void pglColor4f( float r, float g, float b, float a );
void pglColor4ub( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
void pglColor4ubv( uint8_t *v );
void pglTexEnvi( GLenum target, GLenum pname, GLint param );
void pglEnable( GLenum param );
void pglDisable( GLenum param );
void pglEnableClientState( GLenum param );
void pglDisableClientState( GLenum param );

static inline void glTexImage1D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	pglTexImage2D( GL_TEXTURE_2D, level, internalformat, width, 1, border, format, type, pixels );
}
static inline void glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( GL_TEXTURE_2D, level, xoffset, 0, width, 1, format, type, pixels );
} 

static inline void glTexImage3D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	pglTexImage2D( GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels );
}
static inline void glTexSubImage3D( GLenum target, GLint level,
                      GLint xoffset, GLint yoffset,
                      GLint zoffset, GLsizei width,
                      GLsizei height, GLsizei depth,
                      GLenum format,
                      GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, pixels );
}



#endif /* GL_STUBS_H */
