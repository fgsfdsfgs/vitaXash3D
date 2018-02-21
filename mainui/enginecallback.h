/*
enginecallback.h - actual engine callbacks
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

#ifndef ENGINECALLBACKS_H
#define ENGINECALLBACKS_H

// built-in memory manager
#define MALLOC( x )		(*g_ui_engfuncs.pfnMemAlloc)( x, __FILE__, __LINE__ )
#define CALLOC( x, y )	(*g_ui_engfuncs.pfnMemAlloc)((x) * (y), __FILE__, __LINE__ )
#define FREE( x )		(*g_ui_engfuncs.pfnMemFree)( x, __FILE__, __LINE__ )

// screen handlers
#define PIC_Width		(*g_ui_engfuncs.pfnPIC_Width)
#define PIC_Height		(*g_ui_engfuncs.pfnPIC_Height)
#define PIC_EnableScissor	(*g_ui_engfuncs.pfnPIC_EnableScissor)
#define PIC_DisableScissor	(*g_ui_engfuncs.pfnPIC_DisableScissor)
#define FillRGBA		(*g_ui_engfuncs.pfnFillRGBA)
#define GetScreenInfo	(*g_ui_engfuncs.pfnGetScreenInfo)
#define GetGameInfo		(*g_ui_engfuncs.pfnGetGameInfo)
#define CheckGameDll	(*g_ui_engfuncs.pfnCheckGameDll)

#define PIC_SetGamma( x, y )	(*g_ui_engfuncs.pfnProcessImage)( x, y, -1, -1 )
#define PIC_Remap( x, y, z )	(*g_ui_engfuncs.pfnProcessImage)( x, -1.0f, y, z )

#define DRAW_LOGO		(*g_ui_engfuncs.pfnDrawLogo)
#define PRECACHE_LOGO( x )	(*g_ui_engfuncs.pfnDrawLogo)( x, 0, 0, 0, 0 )
#define GetLogoWidth	(*g_ui_engfuncs.pfnGetLogoWidth)
#define GetLogoHeight	(*g_ui_engfuncs.pfnGetLogoHeight)
#define GetLogoLength	(*g_ui_engfuncs.pfnGetLogoLength)

inline HIMAGE PIC_Load( const char *szPicName, long flags = 0 )
{
	return g_ui_engfuncs.pfnPIC_Load( szPicName, NULL, 0, flags );
}

inline HIMAGE PIC_Load( const char *szPicName, const byte *ucRawImage, long ulRawImageSize, long flags = 0 )
{
	return g_ui_engfuncs.pfnPIC_Load( szPicName, ucRawImage, ulRawImageSize, flags );
}

inline int FILE_EXISTS( const char *file, int gamedironly = FALSE )
{
	return g_ui_engfuncs.pfnFileExists( file, gamedironly );
}

#define PIC_Free		(*g_ui_engfuncs.pfnPIC_Free)
#define PLAY_SOUND		(*g_ui_engfuncs.pfnPlayLocalSound)
#define CVAR_REGISTER	(*g_ui_engfuncs.pfnRegisterVariable)
#define CVAR_SET_FLOAT	(*g_ui_engfuncs.pfnCvarSetValue)
#define CVAR_GET_FLOAT	(*g_ui_engfuncs.pfnGetCvarFloat)
#define CVAR_GET_STRING	(*g_ui_engfuncs.pfnGetCvarString)
#define CVAR_SET_STRING	(*g_ui_engfuncs.pfnCvarSetString)
#define CLIENT_COMMAND	(*g_ui_engfuncs.pfnClientCmd)
#define CLIENT_JOIN		(*g_ui_engfuncs.pfnClientJoin)

#define GET_MENU_EDICT	(*g_ui_engfuncs.pfnGetPlayerModel)
#define ENGINE_SET_MODEL	(*g_ui_engfuncs.pfnSetModel)
#define R_ClearScene	(*g_ui_engfuncs.pfnClearScene)
#define R_AddEntity		(*g_ui_engfuncs.CL_CreateVisibleEntity)
#define R_RenderFrame	(*g_ui_engfuncs.pfnRenderScene)

#define LOAD_FILE		(*g_ui_engfuncs.COM_LoadFile)
#define FREE_FILE		(*g_ui_engfuncs.COM_FreeFile)
#define GET_GAME_DIR	(*g_ui_engfuncs.pfnGetGameDir)
#define HOST_ERROR		(*g_ui_engfuncs.pfnHostError)
#define COM_ParseFile	(*g_ui_engfuncs.COM_ParseFile)
#define KEY_SetDest		(*g_ui_engfuncs.pfnSetKeyDest)
#define KEY_ClearStates	(*g_ui_engfuncs.pfnKeyClearStates)
#define KEY_KeynumToString	(*g_ui_engfuncs.pfnKeynumToString)
#define KEY_GetBinding	(*g_ui_engfuncs.pfnKeyGetBinding)
#define KEY_SetBinding	(*g_ui_engfuncs.pfnKeySetBinding)
#define KEY_IsDown		(*g_ui_engfuncs.pfnKeyIsDown)
#define KEY_GetOverstrike	(*g_ui_engfuncs.pfnKeyGetOverstrikeMode)
#define KEY_SetOverstrike	(*g_ui_engfuncs.pfnKeySetOverstrikeMode)
#define Key_GetState	(*g_ui_engfuncs.pfnKeyGetState)
#define SET_CURSOR		(*g_ui_engfuncs.pfnSetCursor)
	
#define Cmd_AddCommand	(*g_ui_engfuncs.pfnAddCommand)
#define Cmd_RemoveCommand	(*g_ui_engfuncs.pfnDelCommand)
#define CMD_ARGC		(*g_ui_engfuncs.pfnCmdArgc)
#define CMD_ARGV		(*g_ui_engfuncs.pfnCmdArgv)
#define Con_Printf		(*g_ui_engfuncs.Con_Printf)
#define Con_NPrintf		(*g_ui_engfuncs.Con_NPrintf)

#define GET_GAMES_LIST	(*g_ui_engfuncs.pfnGetGamesList)
#define BACKGROUND_TRACK	(*g_ui_engfuncs.pfnPlayBackgroundTrack)
#define SHELL_EXECUTE	(*g_ui_engfuncs.pfnShellExecute)
#define HOST_WRITECONFIG	(*g_ui_engfuncs.pfnWriteServerConfig)
#define HOST_CHANGEGAME	(*g_ui_engfuncs.pfnChangeInstance)
#define CHECK_MAP_LIST	(*g_ui_engfuncs.pfnCreateMapsList)
#define HOST_ENDGAME	(*g_ui_engfuncs.pfnHostEndGame)
#define GET_CLIPBOARD	(*g_ui_engfuncs.pfnGetClipboardData)
#define FS_SEARCH		(*g_ui_engfuncs.pfnGetFilesList)
#define MAP_IS_VALID	(*g_ui_engfuncs.pfnIsMapValid)

#define GET_SAVE_COMMENT	(*g_ui_engfuncs.pfnGetSaveComment)
#define GET_DEMO_COMMENT	(*g_ui_engfuncs.pfnGetDemoComment)

#define CL_IsActive()	(g_ui_engfuncs.pfnClientInGame() && !CVAR_GET_FLOAT( "cl_background" ))

inline void PIC_Set( HIMAGE hPic, int r, int g, int b )
{
	g_ui_engfuncs.pfnPIC_Set( hPic, r, g, b, 255 );
}

inline void PIC_Set( HIMAGE hPic, int r, int g, int b, int a )
{
	g_ui_engfuncs.pfnPIC_Set( hPic, r, g, b, a );
}

inline void PIC_Draw( int x, int y, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_Draw( x, y, -1, -1, prc );
}

inline void PIC_Draw( int x, int y, int width, int height )
{
	g_ui_engfuncs.pfnPIC_Draw( x, y, width, height, NULL );
}

inline void PIC_Draw( int x, int y, int width, int height, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_Draw( x, y, width, height, prc );
}

inline void PIC_DrawTrans( int x, int y, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawTrans( x, y, -1, -1, prc );
}

inline void PIC_DrawTrans( int x, int y, int width, int height )
{
	g_ui_engfuncs.pfnPIC_DrawTrans( x, y, width, height, NULL );
}

inline void PIC_DrawTrans( int x, int y, int width, int height, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawTrans( x, y, width, height, prc );
}

inline void PIC_DrawHoles( int x, int y, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawHoles( x, y, -1, -1, prc );
}

inline void PIC_DrawHoles( int x, int y, int width, int height )
{
	g_ui_engfuncs.pfnPIC_DrawHoles( x, y, width, height, NULL );
}

inline void PIC_DrawHoles( int x, int y, int width, int height, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawHoles( x, y, width, height, prc );
}

inline void PIC_DrawAdditive( int x, int y, int width, int height )
{
	g_ui_engfuncs.pfnPIC_DrawAdditive( x, y, width, height, NULL );
}

inline void PIC_DrawAdditive( int x, int y, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawAdditive( x, y, -1, -1, prc );
}

inline void PIC_DrawAdditive( int x, int y, int w, int h, const wrect_t *prc )
{
	g_ui_engfuncs.pfnPIC_DrawAdditive( x, y, w, h, prc );
}

inline void TextMessageSetColor( int r, int g, int b, int alpha = 255 )
{
	g_ui_engfuncs.pfnDrawSetTextColor( r, g, b, alpha );
}

#define TextMessageDrawChar	(*g_ui_engfuncs.pfnDrawCharacter)
#define DrawConsoleString	(*g_ui_engfuncs.pfnDrawConsoleString)
#define GetConsoleStringSize	(*g_ui_engfuncs.pfnDrawConsoleStringLen)
#define ConsoleSetColor	(*g_ui_engfuncs.pfnSetConsoleDefaultColor)

#define RANDOM_LONG		(*g_ui_engfuncs.pfnRandomLong)
#define RANDOM_FLOAT	(*g_ui_engfuncs.pfnRandomFloat)

#define COMPARE_FILE_TIME	(*g_ui_engfuncs.pfnCompareFileTime)

//#define UtfProcessChar		(*g_ui_textfuncs.pfnUtfProcessChar)
//#define UtfMoveLeft		(*g_ui_textfuncs.pfnUtfMoveLeft)
//#define UtfMoveRight		(*g_ui_textfuncs.pfnUtfMoveRight)
//#define EnableTextInput		(*g_ui_textfuncs.pfnEnableTextInput)
inline void EnableTextInput( int enable )
{
	if( g_ui_textfuncs.pfnEnableTextInput )
		g_ui_textfuncs.pfnEnableTextInput( enable );
}
inline int UtfMoveRight( char *str, int pos, int length )
{
	if( g_ui_textfuncs.pfnUtfMoveRight )
		return g_ui_textfuncs.pfnUtfMoveRight( str, pos, length );
	return pos + 1;
}

inline int UtfMoveLeft( char *str, int pos )
{
	if( g_ui_textfuncs.pfnUtfMoveLeft )
		return g_ui_textfuncs.pfnUtfMoveLeft( str, pos );
	return pos - 1;
}
inline int UtfProcessChar( int in )
{
	if( g_ui_textfuncs.pfnUtfProcessChar )
		return g_ui_textfuncs.pfnUtfProcessChar( in );
	return in;
}

#endif//ENGINECALLBACKS_H
