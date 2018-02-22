/*
snd_vita.c - vita sound backend
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
#if XASH_SOUND == SOUND_VITA
#include "sound.h"
#include <vitasdk.h>

#define u64 uint64_t
#define u8 uint8_t

#define SAMPLE_RATE 48000
#define AUDIOSIZE 16384

extern convar_t *s_primary;
extern dma_t dma;

static dma_t *shm;
static u8 *audiobuffer;
static SceRtcTick initial_tick;
static int snd_initialized;
static int chn = -1;
static int stop_audio = false;
static float tickRate;

static int audio_thread( int args, void *argp )
{
	chn = sceAudioOutOpenPort( SCE_AUDIO_OUT_PORT_TYPE_MAIN, AUDIOSIZE / 4, SAMPLE_RATE, SCE_AUDIO_OUT_MODE_STEREO );
	sceAudioOutSetConfig( chn, -1, -1, -1 );
	int vol[] = { 32767, 32767 };
	sceAudioOutSetVolume( chn, SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH, vol );

	while( !stop_audio )
		sceAudioOutOutput( chn, audiobuffer );

	sceAudioOutReleasePort( chn );
	free( audiobuffer );

	sceKernelExitDeleteThread( 0 );
	return 0;
}

qboolean SNDDMA_Init( void *hInst )
{
	snd_initialized = 0;

	audiobuffer = malloc( AUDIOSIZE );

	/* Fill the audio DMA information block */
	shm = &dma;
	shm->format.width = 2;
	shm->format.speed = SAMPLE_RATE;
	shm->format.channels = 2;
	shm->samples = AUDIOSIZE / 2;
	shm->samplepos = 0;
	shm->buffer = audiobuffer;
	shm->sampleframes = shm->samples / shm->format.channels;

	tickRate = 1.0f / sceRtcGetTickResolution();

	SceUID audiothread = sceKernelCreateThread( "XashSnd", (void*)&audio_thread, 0x10000100, 0x10000, 0, 0, NULL );
	int res = sceKernelStartThread( audiothread, sizeof(audiothread), &audiothread );
	if( res != 0 )
	{
		Sys_Error( "snd_vita: Failed to init audio thread (0x%x)", res );
		return 0;
	}

	sceRtcGetCurrentTick( &initial_tick );
	MsgDev( D_INFO, "snd_vita: Sound initialized.\n" );

	shm->initialized = true;
	snd_initialized = 1;
	return 1;
}

int SNDDMA_GetDMAPos(void)
{
	if(!snd_initialized)
		return 0;

	SceRtcTick tick;
	sceRtcGetCurrentTick(&tick);
	const unsigned int deltaTick  = tick.tick - initial_tick.tick;
	const float deltaSecond = deltaTick * tickRate;
	u64 samplepos = deltaSecond * SAMPLE_RATE * 2;
	shm->samplepos = samplepos;
	return samplepos;
}

int SNDDMA_GetSoundtime( void )
{
	static int	buffers, oldsamplepos;
	int		samplepos, fullsamples;
	
	fullsamples = dma.samples / 2;

	// it is possible to miscount buffers
	// if it has wrapped twice between
	// calls to S_Update.  Oh well.
	samplepos = SNDDMA_GetDMAPos();

	if( samplepos < oldsamplepos )
	{
		buffers++; // buffer wrapped

		if( paintedtime > 0x40000000 )
		{	
			// time to chop things off to avoid 32 bit limits
			buffers = 0;
			paintedtime = fullsamples;
			S_StopAllSounds();
		}
	}

	oldsamplepos = samplepos;

	return (buffers * fullsamples + samplepos / 2);
}

/*
==============
SNDDMA_BeginPainting

Makes sure dma.buffer is valid
===============
*/
void SNDDMA_BeginPainting( void )
{
	// TODO: add a mutex here or something
}

/*
==============
SNDDMA_Submit

Send sound to device if buffer isn't really the dma buffer
Also unlocks the dsound buffer
===============
*/
void SNDDMA_Submit( void )
{

}

/*
===========
S_PrintDeviceName
===========
*/
void S_PrintDeviceName( void )
{
	Msg( "Audio: Vita\n" );
}

/*
===========
S_Activate
Called when the main window gains or loses focus.
The window have been destroyed and recreated
between a deactivate and an activate.
===========
*/
void S_Activate( qboolean active )
{

}

/*
==============
SNDDMA_Shutdown

Reset the sound device for exiting
===============
*/
void SNDDMA_Shutdown(void)
{
	if( snd_initialized )
	{
		stop_audio = true;
		chn = -1;
	}
}

#endif
