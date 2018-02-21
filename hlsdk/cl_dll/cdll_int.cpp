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
//
//  cdll_int.c
//
// this implementation handles the linking of the engine to the DLL
//

#include "hud.h"
#include "cl_util.h"
#include "netadr.h"
#include "parsemsg.h"

#if defined(GOLDSOURCE_SUPPORT) && (defined(_WIN32) || defined(__linux__) || defined(__APPLE__)) && (defined(__i386) || defined(_M_IX86))
#define USE_VGUI_FOR_GOLDSOURCE_SUPPORT
#include "VGUI_Panel.h"
#include "VGUI_App.h"
#endif

extern "C"
{
#include "pm_shared.h"
}

#include <string.h>

cl_enginefunc_t gEngfuncs;
CHud gHUD;
mobile_engfuncs_t *gMobileEngfuncs = NULL;

extern "C" int g_bhopcap;
void InitInput( void );
void EV_HookEvents( void );
void IN_Commands( void );

int __MsgFunc_Bhopcap( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	g_bhopcap = READ_BYTE();

	return 1;
}

/*
========================== 
    Initialize

Called when the DLL is first loaded.
==========================
*/
extern "C" 
{
int		DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
int		DLLEXPORT HUD_VidInit( void );
void	DLLEXPORT HUD_Init( void );
int		DLLEXPORT HUD_Redraw( float flTime, int intermission );
int		DLLEXPORT HUD_UpdateClientData( client_data_t *cdata, float flTime );
void	DLLEXPORT HUD_Reset ( void );
void	DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server );
void	DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove );
char	DLLEXPORT HUD_PlayerMoveTexture( char *name );
int		DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
int		DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs );
void	DLLEXPORT HUD_Frame( double time );
void	DLLEXPORT HUD_VoiceStatus(int entindex, qboolean bTalking);
void	DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf );
void	DLLEXPORT HUD_MobilityInterface( mobile_engfuncs_t *gpMobileEngfuncs );
}

/*
================================
HUD_GetHullBounds

  Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	int iret = 0;

	switch( hullnumber )
	{
	case 0:				// Normal player
		Vector( -16, -16, -36 ).CopyToArray(mins);
		Vector( 16, 16, 36 ).CopyToArray(maxs);
		iret = 1;
		break;
	case 1:				// Crouched player
		Vector( -16, -16, -18 ).CopyToArray(mins);
		Vector( 16, 16, 18 ).CopyToArray(maxs);
		iret = 1;
		break;
	case 2:				// Point based hull
		Vector( 0, 0, 0 ).CopyToArray(mins);
		Vector( 0, 0, 0 ).CopyToArray(maxs);
		iret = 1;
		break;
	}

	return iret;
}

/*
================================
HUD_ConnectionlessPacket

 Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
  size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	// Parse stuff from args
	int max_buffer_size = *response_buffer_size;

	// Zero it out since we aren't going to respond.
	// If we wanted to response, we'd write data into response_buffer
	*response_buffer_size = 0;

	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead.
	return 0;
}

void DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
	PM_Init( ppmove );
}

char DLLEXPORT HUD_PlayerMoveTexture( char *name )
{
	return PM_FindTextureType( name );
}

void DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server )
{
	PM_Move( ppmove, server );
}

int DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	gEngfuncs = *pEnginefuncs;

	if( iVersion != CLDLL_INTERFACE_VERSION )
		return 0;

	memcpy( &gEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t) );

	EV_HookEvents();

	return 1;
}

/*
=================
HUD_GetRect

VGui stub
=================
*/
int *HUD_GetRect( void )
{
	static int extent[4];

	extent[0] = gEngfuncs.GetWindowCenterX() - ScreenWidth / 2;
	extent[1] = gEngfuncs.GetWindowCenterY() - ScreenHeight / 2;
	extent[2] = gEngfuncs.GetWindowCenterX() + ScreenWidth / 2;
	extent[3] = gEngfuncs.GetWindowCenterY() + ScreenHeight / 2;

	return extent;
}

#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
class TeamFortressViewport : public vgui::Panel
{
public:
	TeamFortressViewport(int x,int y,int wide,int tall);
	void Initialize( void );

	virtual void paintBackground();
	void *operator new( size_t stAllocateBlock );
};

static TeamFortressViewport* gViewPort = NULL;

TeamFortressViewport::TeamFortressViewport(int x, int y, int wide, int tall) : Panel(x, y, wide, tall)
{
	gViewPort = this;
	Initialize();
}

void TeamFortressViewport::Initialize()
{
	//vgui::App::getInstance()->setCursorOveride( vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_none) );
}

void TeamFortressViewport::paintBackground()
{
//	int wide, tall;
//	getParent()->getSize( wide, tall );
//	setSize( wide, tall );
	gEngfuncs.VGui_ViewportPaintBackground(HUD_GetRect());
}

void *TeamFortressViewport::operator new( size_t stAllocateBlock )
{
	void *mem = ::operator new( stAllocateBlock );
	memset( mem, 0, stAllocateBlock );
	return mem;
}
#endif

/*
==========================
	HUD_VidInit

Called when the game initializes
and whenever the vid_mode is changed
so the HUD can reinitialize itself.
==========================
*/

int DLLEXPORT HUD_VidInit( void )
{
	gHUD.VidInit();
#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
	vgui::Panel* root=(vgui::Panel*)gEngfuncs.VGui_GetPanel();
	if (root) {
		gEngfuncs.Con_Printf( "Root VGUI panel exists\n" );
		root->setBgColor(128,128,0,0);

		if (gViewPort != NULL)
		{
			gViewPort->Initialize();
		}
		else
		{
			gViewPort = new TeamFortressViewport(0,0,root->getWide(),root->getTall());
			gViewPort->setParent(root);
		}
	} else {
		gEngfuncs.Con_Printf( "Root VGUI panel does not exist\n" );
	}
#endif
	return 1;
}

/*
==========================
	HUD_Init

Called whenever the client connects
to a server.  Reinitializes all 
the hud variables.
==========================
*/

void DLLEXPORT HUD_Init( void )
{
	InitInput();
	gHUD.Init();

	gEngfuncs.pfnHookUserMsg( "Bhopcap", __MsgFunc_Bhopcap );
}

/*
==========================
	HUD_Redraw

called every screen frame to
redraw the HUD.
===========================
*/

int DLLEXPORT HUD_Redraw( float time, int intermission )
{
	gHUD.Redraw( time, intermission );

	return 1;
}

/*
==========================
	HUD_UpdateClientData

called every time shared client
dll/engine data gets changed,
and gives the cdll a chance
to modify the data.

returns 1 if anything has been changed, 0 otherwise.
==========================
*/

int DLLEXPORT HUD_UpdateClientData( client_data_t *pcldata, float flTime )
{
	IN_Commands();

	return gHUD.UpdateClientData( pcldata, flTime );
}

/*
==========================
	HUD_Reset

Called at start and end of demos to restore to "non"HUD state.
==========================
*/

void DLLEXPORT HUD_Reset( void )
{
	gHUD.VidInit();
}

/*
==========================
HUD_Frame

Called by engine every frame that client .dll is loaded
==========================
*/

void DLLEXPORT HUD_Frame( double time )
{
#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
	if (!gViewPort)
		gEngfuncs.VGui_ViewportPaintBackground(HUD_GetRect());
#else
	gEngfuncs.VGui_ViewportPaintBackground(HUD_GetRect());
#endif
}

/*
==========================
HUD_VoiceStatus

Called when a player starts or stops talking.
==========================
*/

void DLLEXPORT HUD_VoiceStatus( int entindex, qboolean bTalking )
{

}

/*
==========================
HUD_DirectorEvent

Called when a director event message was received
==========================
*/

void DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf )
{
	 gHUD.m_Spectator.DirectorMessage( iSize, pbuf );
}

void DLLEXPORT HUD_MobilityInterface( mobile_engfuncs_t *gpMobileEngfuncs )
{
	if( gpMobileEngfuncs->version != MOBILITY_API_VERSION )
		return;
	gMobileEngfuncs = gpMobileEngfuncs;
}

bool isXashFWGS()
{
	return gMobileEngfuncs != NULL;
}

#ifdef __vita__

//===============================
// exports for vita dlsym() shit
//===============================

// for some reason these weren't in the exports block above
extern "C"
{
void DLLEXPORT HUD_Shutdown( void );
void DLLEXPORT HUD_PostRunCmd( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
int DLLEXPORT HUD_Key_Event( int down, int keynum, const char *pszCurrentBinding );
int DLLEXPORT HUD_AddEntity( int type, struct cl_entity_s *ent, const char *modelname );
void DLLEXPORT HUD_CreateEntities( void );
void DLLEXPORT HUD_StudioEvent( const struct mstudioevent_s *event, const struct cl_entity_s *entity );
void DLLEXPORT HUD_TxferLocalOverrides( struct entity_state_s *state, const struct clientdata_s *client );
void DLLEXPORT HUD_ProcessPlayerState( struct entity_state_s *dst, const struct entity_state_s *src );
void DLLEXPORT HUD_TxferPredictionData ( struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd );
void DLLEXPORT HUD_TempEntUpdate( double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int ( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void ( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
int DLLEXPORT HUD_GetStudioModelInterface( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio );
void DLLEXPORT HUD_DrawNormalTriangles( void );
void DLLEXPORT HUD_DrawTransparentTriangles( void );
cl_entity_t DLLEXPORT *HUD_GetUserEntity( int index );
void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer );
void DLLEXPORT CL_CreateMove( float frametime, struct usercmd_s *cmd, int active );
struct kbutton_s DLLEXPORT *KB_Find( const char *name );
void DLLEXPORT CAM_Think( void );
int DLLEXPORT CL_IsThirdPerson( void );
void DLLEXPORT CL_CameraOffset( float *ofs );
void DLLEXPORT IN_ClientMoveEvent( float forwardmove, float sidemove );
void DLLEXPORT IN_ClientLookEvent( float relyaw, float relpitch );
void DLLEXPORT IN_MouseEvent_CL( int mstate );
void DLLEXPORT IN_ClearStates( void );
void DLLEXPORT IN_ActivateMouse_CL( void );
void DLLEXPORT IN_DeactivateMouse_CL( void );
void DLLEXPORT IN_Accumulate( void );
void DLLEXPORT V_CalcRefdef_CL( struct ref_params_s *pparams );

typedef struct dllexport_s
{
	const char *name;
	void *func;
} dllexport_t;

dllexport_t vita_client_exports[] = {
	{ "Initialize", (void*)Initialize },
	{ "HUD_VidInit", (void*)HUD_VidInit },
	{ "HUD_Init", (void*)HUD_Init },
	{ "HUD_Shutdown", (void*)HUD_Shutdown },
	{ "HUD_Redraw", (void*)HUD_Redraw },
	{ "HUD_UpdateClientData", (void*)HUD_UpdateClientData },
	{ "HUD_Reset", (void*)HUD_Reset },
	{ "HUD_PlayerMove", (void*)HUD_PlayerMove },
	{ "HUD_PlayerMoveInit", (void*)HUD_PlayerMoveInit },
	{ "HUD_PlayerMoveTexture", (void*)HUD_PlayerMoveTexture },
	{ "HUD_ConnectionlessPacket", (void*)HUD_ConnectionlessPacket },
	{ "HUD_GetHullBounds", (void*)HUD_GetHullBounds },
	{ "HUD_Frame", (void*)HUD_Frame },
	{ "HUD_PostRunCmd", (void*)HUD_PostRunCmd },
	{ "HUD_Key_Event", (void*)HUD_Key_Event },
	{ "HUD_AddEntity", (void*)HUD_AddEntity },
	{ "HUD_CreateEntities", (void*)HUD_CreateEntities },
	{ "HUD_StudioEvent", (void*)HUD_StudioEvent },
	{ "HUD_TxferLocalOverrides", (void*)HUD_TxferLocalOverrides },
	{ "HUD_ProcessPlayerState", (void*)HUD_ProcessPlayerState },
	{ "HUD_TxferPredictionData", (void*)HUD_TxferPredictionData },
	{ "HUD_TempEntUpdate", (void*)HUD_TempEntUpdate },
	{ "HUD_DrawNormalTriangles", (void*)HUD_DrawNormalTriangles },
	{ "HUD_DrawTransparentTriangles", (void*)HUD_DrawTransparentTriangles },
	{ "HUD_GetUserEntity", (void*)HUD_GetUserEntity },
	{ "Demo_ReadBuffer", (void*)Demo_ReadBuffer },
	{ "CAM_Think", (void*)CAM_Think },
	{ "CL_IsThirdPerson", (void*)CL_IsThirdPerson },
	{ "CL_CameraOffset", (void*)CL_CameraOffset },
	{ "CL_CreateMove", (void*)CL_CreateMove },
	{ "IN_ActivateMouse", (void*)IN_ActivateMouse_CL },
	{ "IN_DeactivateMouse", (void*)IN_DeactivateMouse_CL },
	{ "IN_MouseEvent", (void*)IN_MouseEvent_CL },
	{ "IN_Accumulate", (void*)IN_Accumulate },
	{ "IN_ClearStates", (void*)IN_ClearStates },
	{ "V_CalcRefdef", (void*)V_CalcRefdef_CL },
	{ "KB_Find", (void*)KB_Find },
	{ "HUD_GetStudioModelInterface", (void*)HUD_GetStudioModelInterface },
	{ "HUD_DirectorMessage", (void*)HUD_DirectorMessage },
	{ "HUD_VoiceStatus", (void*)HUD_VoiceStatus },
	{ "IN_ClientMoveEvent", (void*)IN_ClientMoveEvent}, // Xash3D ext
	{ "IN_ClientLookEvent", (void*)IN_ClientLookEvent}, // Xash3D ext
	{ NULL, NULL },
};

int dll_register( const char *name, dllexport_t *exports );

int vita_installdll_client( void )
{
	return dll_register( "client", vita_client_exports );
}

}

#endif
