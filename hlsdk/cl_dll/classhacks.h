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
#pragma once

#ifndef CLASSHACKS_H
#define CLASSHACKS_H

#if defined CLIENT_DLL

// HACKHACKHACK:
// this redefines all the symbol names that client.dll and game.dll share
// to make isolated copies of them for each dll
// I couldn't think of anything better
// I'm afraid sooner or later this list is going to get so large that the
// binary size will exceed all acceptable limits
// include this in your mod before anything else, preferably using `-include`

#define CBaseEntity CLDLL_CBaseEntity
#define CBaseDelay CLDLL_CBaseDelay
#define CBaseAnimating CLDLL_CBaseAnimating
#define CBaseToggle CLDLL_CBaseToggle
#define CBaseMonster CLDLL_CBaseMonster
#define CBasePlayer CLDLL_CBasePlayer
#define CBasePlayerItem CLDLL_CBasePlayerItem
#define CBasePlayerWeapon CLDLL_CBasePlayerWeapon
#define CBasePlayerAmmo CLDLL_CBasePlayerAmmo

#define CSave CLDLL_CSave
#define CRestore CLDLL_CRestore

#define CWeaponBox CLDLL_CWeaponBox
#define CGrenade CLDLL_CGrenade
#define CGlock CLDLL_CGlock
#define CCrowbar CLDLL_CCrowbar
#define CPython CLDLL_CPython
#define CMP5 CLDLL_CMP5
#define CCrossbow CLDLL_CCrossbow
#define CShotgun CLDLL_CShotgun
#define CRpg CLDLL_CRpg
#define CGauss CLDLL_CGauss
#define CEgon CLDLL_CEgon
#define CHgun CLDLL_CHgun
#define CHandGrenade CLDLL_CHandGrenade
#define CSatchel CLDLL_CSatchel
#define CSatchelCharge CLDLL_CSatchelCharge
#define CTripmine CLDLL_CTripmine
#define CSqueak CLDLL_CSqueak
#define CRpgRocket CLDLL_CRpgRocket
#define CLaserSpot CLDLL_CLaserSpot

#define CSprite CLDLL_CSprite
#define CBeam CLDLL_CBeam
#define CSoundEnt CLDLL_CSoundEnt

#define EMIT_SOUND_DYN CLDLL_EMIT_SOUND_DYN

#define UTIL_PrecacheOther CLDLL_UTIL_PrecacheOther
#define UTIL_BloodDrips CLDLL_UTIL_BloodDrips
#define UTIL_DecalTrace CLDLL_UTIL_DecalTrace
#define UTIL_GunshotDecalTrace CLDLL_UTIL_GunshotDecalTrace
#define UTIL_MakeVectors CLDLL_UTIL_MakeVectors
#define UTIL_IsValidEntity CLDLL_UTIL_IsValidEntity
#define UTIL_SetOrigin CLDLL_UTIL_SetOrigin
#define UTIL_GetNextBestWeapon CLDLL_UTIL_GetNextBestWeapon
#define UTIL_LogPrintf CLDLL_UTIL_LogPrintf
#define UTIL_ClientPrintAll CLDLL_UTIL_ClientPrintAll
#define UTIL_Remove CLDLL_UTIL_Remove
#define UTIL_SetSize CLDLL_UTIL_SetSize
#define UTIL_FindEntityInSphere CLDLL_UTIL_FindEntityInSphere
#define UTIL_VecToAngles CLDLL_UTIL_VecToAngles
#define UTIL_WeaponTimeBase CLDLL_UTIL_WeaponTimeBase
#define UTIL_StringToVector CLDLL_UTIL_StringToVector
#define UTIL_TraceLine CLDLL_UTIL_TraceLine
#define UTIL_SharedRandomFloat CLDLL_UTIL_SharedRandomFloat
#define UTIL_SharedRandomLong CLDLL_UTIL_SharedRandomLong
#define U_Srand CLDLL_U_Srand
#define U_Random CLDLL_U_Random

#define gSkillData CLDLL_gSkillData
#define gEntvarsDescription CLDLL_gEntvarsDescription
#define g_vecZero CLDLL_g_VecZero
#define gmsgWeapPickup CLDLL_gmsgWeapPickup
#define seed_table CLDLL_seed_table

#define ClientPrint CLDLL_ClientPrint
#define ShouldSimplify CLDLL_ShouldSimplify
#define TrainSpeed CLDLL_TrainSpeed
#define ClearMultiDamage CLDLL_ClearMultiDamage
#define ApplyMultiDamage CLDLL_ApplyMultiDamage
#define AddMultiDamage CLDLL_AddMultiDamage
#define SpawnBlood CLDLL_SpawnBlood
#define DamageDecal CLDLL_DamageDecal
#define DecalGunshot CLDLL_DecalGunshot
#define EjectBrass CLDLL_EjectBrass
#define AddAmmoNameToAmmoRegistry CLDLL_AddAmmoNameToAmmoRegistry
#define RadiusDamage CLDLL_RadiusDamage
#define FindHullIntersection CLDLL_FindHullIntersection

// apparently this also needs to be in both dlls
#define PM_ViewEntity CLDLL_PM_ViewEntity
#define PM_DrawBBox CLDLL_PM_DrawBBox
#define PM_ParticleLine CLDLL_PM_ParticleLine
#define PM_ShowClipBox CLDLL_PM_ShowClipBox
#define PM_Init CLDLL_PM_Init
#define PM_Move CLDLL_PM_Move
#define PM_FindTextureType CLDLL_PM_FindTextureType
#define PM_SwapTextures CLDLL_PM_SwapTextures
#define PM_SortTextures CLDLL_PM_SortTextures
#define PM_InitTextureTypes CLDLL_PM_InitTextureTypes
#define PM_PlayStepSound CLDLL_PM_PlayStepSound
#define PM_MapTextureTypeStepType CLDLL_PM_MapTextureTypeStepType
#define PM_CatagorizeTextureType CLDLL_PM_CatagorizeTextureType
#define PM_UpdateStepSound CLDLL_PM_UpdateStepSound
#define PM_AddToTouched CLDLL_PM_AddToTouched
#define PM_CheckVelocity CLDLL_PM_CheckVelocity
#define PM_ClipVelocity CLDLL_PM_ClipVelocity
#define PM_AddCorrectGravity CLDLL_PM_AddCorrectGravity
#define PM_FixupGravityVelocity CLDLL_PM_FixupGravityVelocity
#define PM_FlyMove CLDLL_PM_FlyMove
#define PM_Accelerate CLDLL_PM_Accelerate
#define PM_WalkMove CLDLL_PM_WalkMove
#define PM_Friction CLDLL_PM_Friction
#define PM_AirAccelerate CLDLL_PM_AirAccelerate
#define PM_WaterMove CLDLL_PM_WaterMove
#define PM_AirMove CLDLL_PM_AirMove
#define PM_InWater CLDLL_PM_InWater
#define PM_CheckWater CLDLL_PM_CheckWater
#define PM_CatagorizePosition CLDLL_PM_CatagorizePosition
#define PM_GetRandomStuckOffsets CLDLL_PM_GetRandomStuckOffsets
#define PM_ResetStuckOffsets CLDLL_PM_ResetStuckOffsets
#define PM_CheckStuck CLDLL_PM_CheckStuck
#define PM_SpectatorMove CLDLL_PM_SpectatorMove
#define PM_SplineFraction CLDLL_PM_SplineFraction
#define PM_FixPlayerCrouchStuck CLDLL_PM_FixPlayerCrouchStuck
#define PM_UnDuck CLDLL_PM_UnDuck
#define PM_Duck CLDLL_PM_Duck
#define PM_LadderMove CLDLL_LadderMove
#define PM_Ladder CLDLL_PM_Ladder
#define PM_WaterJump CLDLL_PM_WaterJump
#define PM_AddGravity CLDLL_PM_AddGravity
#define PM_PushEntity CLDLL_PM_PushEntity
#define PM_Physics_Toss CLDLL_PM_Physics_Toss
#define PM_NoClip CLDLL_PM_NoClip
#define PM_PreventMegaBunnyJumping CLDLL_PM_PreventMegaBunnyJumping
#define PM_Jump CLDLL_PM_Jump
#define PM_CheckWaterJump CLDLL_PM_CheckWaterJump
#define PM_CheckFalling CLDLL_PM_CheckFalling
#define PM_PlayWaterSounds CLDLL_PM_PlayWaterSounds
#define PM_CalcRoll CLDLL_PM_CalcRoll
#define PM_DropPunchAngle CLDLL_PM_DropPunchAngle
#define PM_CheckParamters CLDLL_PM_CheckParamters
#define PM_ReduceTimers CLDLL_PM_ReduceTimers
#define PM_PlayerMove CLDLL_PM_PlayerMove
#define PM_CreateStuckTable CLDLL_PM_CreateStuckTable
#define PM_GetVisEntInfo CLDLL_PM_GetVisEntInfo
#define PM_GetPhysEntInfo CLDLL_PM_GetPhysEntInfo

#define g_bhopcap CLDLL_g_bhopcap
#define g_onladder CLDLL_g_onladder
#define pmove CLDLL_pmove
#define movevars CLDLL_movevars

#endif

#endif