/*
in_vita.c - vita input backend
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

#if XASH_INPUT == INPUT_VITA

#include "input.h"
#include "joyinput.h"
#include "touch.h"
#include "client.h"
#include "in_vita.h"
#include <vitasdk.h>

static int vita_deadzone_l = 30;
static int vita_deadzone_r = 30;
static SceCtrlData pad, pad_old;

static SceTouchData touch_front, touch_back;

static int backtouch_key = K_MOUSE2;
static int backtouch, backtouch_old;

static qboolean fronttouch;
static float fronttouch_x, fronttouch_y;

qboolean vita_keyboard_on = false;
static char input_concmd[128];
static uint16_t input_text[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
static uint16_t initial_text[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
static uint16_t input_title[] =
{
	'I', 'n', 'p', 'u', 't', ' ',
	'c', 'o', 'm', 'm', 'a', 'n', 'd', 0
};
static char *input_target;
static int input_target_sz;

typedef struct buttonmapping_s
{
	SceCtrlButtons btn;
	int key;
} buttonmapping_t;

static buttonmapping_t btnmap[12] =
{
	{ SCE_CTRL_SELECT, '~' },
	{ SCE_CTRL_START, K_ESCAPE },
	{ SCE_CTRL_UP, K_UPARROW },
	{ SCE_CTRL_RIGHT, K_RIGHTARROW },
	{ SCE_CTRL_DOWN, K_DOWNARROW },
	{ SCE_CTRL_LEFT, K_LEFTARROW },
	{ SCE_CTRL_LTRIGGER, K_ALT },
	{ SCE_CTRL_RTRIGGER, K_CTRL },
	{ SCE_CTRL_TRIANGLE, K_SHIFT },
	{ SCE_CTRL_CIRCLE, K_BACKSPACE },
	{ SCE_CTRL_CROSS, K_ENTER },
	{ SCE_CTRL_SQUARE, K_SPACE },
};

static inline void ascii2utf( uint16_t *dst, char *src, int dstsize )
{
	if( !src || !dst ) return;
	while( *src && (dstsize-- > 0) )
		*(dst++) = (*src++);
	*dst = 0x00;
}

static inline void utf2ascii( char *dst, uint16_t *src, int dstsize )
{
	if( !src || !dst ) return;
	while( *src && (dstsize-- > 0) )
		*(dst++) = (*(src++)) & 0xFF;
	*dst = 0x00;
}


static void RescaleAnalog( int *x, int *y, int dead )
{
	//radial and scaled deadzone
	//http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
	float analogX = (float)*x;
	float analogY = (float)*y;
	float deadZone = (float)dead;
	float maximum = 128.0f;
	float magnitude = sqrt( analogX * analogX + analogY * analogY );
	if( magnitude >= deadZone )
	{
		float scalingFactor = maximum / magnitude * ( magnitude - deadZone ) / ( maximum - deadZone );
		*x = (int)( analogX * scalingFactor );
		*y = (int)( analogY * scalingFactor );
	}
	else
	{
		*x = 0;
		*y = 0;
	}
}


static inline void UpdateAxes( void )
{
	int left_x = pad.lx - 127;
	int left_y = pad.ly - 127;
	int right_x = pad.rx - 127;
	int right_y = pad.ry - 127;

	if( abs( left_x ) < vita_deadzone_l ) left_x = 0;
	if( abs( left_y ) < vita_deadzone_l ) left_y = 0;

	RescaleAnalog( &right_x, &right_y, vita_deadzone_r );

	if( left_x < -128 ) left_x = -128;
	else if( left_x > 127 ) left_x = 127;
	if( left_y < -128 ) left_y = -128;
	else if( left_y > 127 ) left_y = 127;

	if( right_x < -127 ) right_x = -127;
	else if( right_x > 128 ) right_x = 128;
	if( right_y < -127 ) right_y = -127;
	else if( right_y > 128 ) right_y = 128;

	Joy_AxisMotionEvent( 0, 0, left_x * 256 );
	Joy_AxisMotionEvent( 0, 1, left_y * 256 );
	Joy_AxisMotionEvent( 0, 2, right_x * -256 );
	Joy_AxisMotionEvent( 0, 3, right_y * -256 );
}

static inline void UpdateButtons( void )
{
	if( cls.key_dest == key_console && ( pad.buttons & SCE_CTRL_LTRIGGER ) )
	{
		Vita_IN_OpenKeyboard( input_concmd, sizeof(input_concmd) );
	}
	else
	{
		for( int i = 0; i < 12; ++i )
			if( ( pad.buttons & btnmap[i].btn ) != ( pad_old.buttons & btnmap[i].btn ) )
				Key_Event( btnmap[i].key, !!( pad.buttons & btnmap[i].btn ) );
	}
}

static inline void UpdateTouch( void )
{
	// use front touchpanel as a regular touch device
	if( touch_front.reportNum > 0 )
	{
		float x = touch_front.report[0].x / 1980.f;
		float y = touch_front.report[0].y / 1088.f;
		if( fronttouch )
		{
			float dx = x - fronttouch_x;
			float dy = y - fronttouch_y;
			IN_TouchEvent( event_motion, 0, x, y, dx, dy );
		}
		else
		{
			IN_TouchEvent( event_down, 0, x, y, 0.f, 0.f );
			fronttouch = true;
		}
		fronttouch_x = x;
		fronttouch_y = y;
	}
	else if( fronttouch )
	{
		IN_TouchEvent( event_up, 0, fronttouch_x, fronttouch_y, 0.f, 0.f );
		fronttouch = false;
	}

	// use back touchpanel as a button
	backtouch = ( touch_back.reportNum > 0 );
	if( backtouch != backtouch_old )
		Key_Event( backtouch_key, backtouch );
}

static inline void UpdateKeyboard( void )
{
	static SceImeDialogResult result;
	SceCommonDialogStatus status = sceImeDialogGetStatus( );
	if( status == 2 )
	{
		memset( &result, 0, sizeof(SceImeDialogResult) );
		sceImeDialogGetResult( &result );
		if( result.button == SCE_IME_DIALOG_BUTTON_ENTER )
		{
			input_target[0] = 0;
			utf2ascii( input_target, input_text, input_target_sz );
			if( input_target == input_concmd && input_target[0] )
				Cbuf_AddText( input_target ), Cbuf_AddText( "\n" );
		}
		sceImeDialogTerm( );
		vita_keyboard_on = false;
	}
}

void Vita_IN_OpenKeyboard( char *target, int target_sz )
{
	if( vita_keyboard_on ) return;
	if( !target || target_sz < 2 ) return;

	SceImeDialogParam param;
	sceImeDialogParamInit( &param );
	param.supportedLanguages = 0x0001FFFF;
	param.languagesForced = SCE_TRUE;
	param.type = SCE_IME_TYPE_BASIC_LATIN;
	param.title = input_title;
	param.maxTextLength = target_sz - 1;
	param.initialText = initial_text;
	param.inputTextBuffer = input_text;

	input_target = target;
	input_target_sz = target_sz;
	vita_keyboard_on = true;

	sceImeDialogInit(&param);
}

void Vita_IN_Init( void )
{
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG_WIDE );
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, 1 );
	sceTouchSetSamplingState( SCE_TOUCH_PORT_BACK, 1 );
}

void Vita_IN_Frame( void )
{
	pad_old = pad;
	backtouch_old = backtouch;

	sceKernelPowerTick( 0 );
	sceCtrlPeekBufferPositive( 0, &pad, 1 );
	sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch_front, 1 );
	sceTouchPeek( SCE_TOUCH_PORT_BACK, &touch_back, 1 );

	if( vita_keyboard_on )
	{
		UpdateKeyboard( );
	}
	else
	{
		UpdateButtons( );
		UpdateAxes( );
		UpdateTouch( );
	}
}

#endif // INPUT_VITA
