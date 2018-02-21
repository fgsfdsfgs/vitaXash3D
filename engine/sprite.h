/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef SPRITE_H
#define SPRITE_H

/*
==============================================================================

SPRITE MODELS

.spr extended version (Half-Life compatible sprites with some Xash3D extensions)
==============================================================================
*/

#define IDSPRITEHEADER	(('P'<<24)+('S'<<16)+('D'<<8)+'I')	// little-endian "IDSP"
#define SPRITE_VERSION	2				// Half-Life sprites

typedef enum
{
	ST_SYNC = 0,
	ST_RAND
} synctype_t;

typedef enum
{
	FRAME_SINGLE = 0,
	FRAME_GROUP,
	FRAME_ANGLED			// Xash3D ext
} frametype_t;

typedef enum
{
	SPR_NORMAL = 0,
	SPR_ADDITIVE,
	SPR_INDEXALPHA,
	SPR_ALPHTEST,
} drawtype_t;

typedef enum
{
	SPR_FWD_PARALLEL_UPRIGHT = 0,
	SPR_FACING_UPRIGHT,
	SPR_FWD_PARALLEL,
	SPR_ORIENTED,
	SPR_FWD_PARALLEL_ORIENTED,
} angletype_t; 

typedef enum
{
	SPR_CULL_FRONT = 0,			// oriented sprite will be draw with one face
	SPR_CULL_NONE,			// oriented sprite will be draw back face too
} facetype_t;

// cross-platform they said
// backend system they said
// why the FUCK were you using enums in structures that are copied
// from buffers with memcpy?! the size of all these enums happens to be 1 byte
// when you build this shit with vitasdk, which means I'll have to go all over
// the fucking code in search of bullshit like this and pluck it out, because
// there's no way to set the default size to 4, and even if there was, it'd
// probably break binary compatibility with the sdk libs

typedef struct
{
	int32_t		ident;		// LittleLong 'ISPR'
	int32_t		version;		// current version 2
	/*angletype_t*/int32_t	type;		// camera align
	/*drawtype_t*/int32_t	texFormat;	// rendering mode
	int32_t		boundingradius;	// quick face culling
	int32_t		bounds[2];	// mins\maxs
	int32_t		numframes;	// including groups
	/*facetype_t*/int32_t	facetype;		// cullface (Xash3D ext)
	/*synctype_t*/int32_t	synctype;		// animation synctype
} dsprite_t;

typedef struct
{
	int		origin[2];
	int		width;
	int		height;
} dspriteframe_t;

typedef struct
{
	int		numframes;
} dspritegroup_t;

typedef struct
{
	float		interval;
} dspriteinterval_t;

typedef struct
{
	/*frametype_t*/int	type;
} dframetype_t;

#endif//SPRITE_H