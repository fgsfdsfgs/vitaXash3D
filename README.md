# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This is currently very broken and not user-friendly at all. Expect severe graphics glitches, crashes and gameplay oddities.

HLSDK is statically linked into the engine executable, so you can only use this with Half-Life right now.

Any other GoldSrc mod/game that has custom DLLs would require some modifications to be able to hardlink in the same way, I'll write a guide on that later.

Mod support for mods that don't have their own DLLs (e. g. USS Darkstar) is currently broken as well.

## Requirements
- VitaSDK (in PATH and with VITASDK set);
- [vitaGL](https://github.com/Rinnegatamante/vitaGL).

## Building
Run `make` in this directory. This should produce a VPK.

## Running
1. Copy the `valve` directory from your Half-Life installation (preferably the Steam version) to `ux0:/data/xash3d/` on your Vita. If you don't have access to a Half-Life installation,
you can use the [data files](https://github.com/fgsfdsfgs/vitaXash3D/releases/download/v1/uplink.zip) from the free demo version of HL, Half-Life Uplink.
2. Copy the `build/data` directory to `ux0:/` on your Vita. Replace everything if asked.
3. Delete everything from `valve/cl_dlls` and `valve/dlls`.
4. Install the VPK obtained after building the port.
5. Run it.

Selecting the "Dev Mode" icon enables advanced logging and debug features, so use that for debugging. The log will be saved to `ux0:/data/xash3d/engine.log`.

Keep in mind that the game can hang for a long time when saving or autosaving (see issue #7), so don't kill it if it suddenly hangs when you enter a map or cross a checkpoint.

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice;
- ArkSource for the Live Area stuff.
