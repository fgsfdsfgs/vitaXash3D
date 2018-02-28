# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This port is still a work in progress. Expect graphics and sound glitches, crashes and gameplay oddities.

HLSDK is statically linked into the engine executable, so for now you can only use this with Half-Life, custom HL maps and mods that don't have custom game code (e. g. USS Darkstar).

Any other GoldSrc mod/game that has custom DLLs would require some modifications to be able to hardlink in the same way, I'll write a guide on that later.

## Requirements
- VitaSDK (in PATH and with VITASDK set);
- [vitaGL](https://github.com/Rinnegatamante/vitaGL).

If you're experiencing bugs with the latest version of vitaGL, try [this version](https://github.com/Rinnegatamante/vitaGL/tree/cdfb4290ead01d39611b46846a23ef66a258b3ce).

If HL crashes when loading the first map in Unforeseen Consequences or you're experiencing similar crashes on map loads, try increasing `TEXTURES_NUM` in `vitaGL.c` to 1536.

## Building
Run `make` in this directory. This should produce a VPK.

## Running
1. Copy the `valve` directory from your Half-Life installation (preferably the Steam version) to `ux0:/data/xash3d/` on your Vita. If you don't have access to a Half-Life installation,
you can use the data files from the free demo version of HL, Half-Life Uplink. In that case, just extract [this archive](https://drive.google.com/file/d/18DCj4KFS7GMaBZyQ3BPgdM8s1mA7AWED/view) to `ux0:/` on your Vita.
2. Copy the `build/data` directory to `ux0:/` on your Vita. Replace everything if asked.
3. Install the VPK obtained after building the port.
4. Run it. If you only have Uplink installed, make sure that you select `Half-Life Uplink` in the launcher.

#### Notes
The console log is saved to `ux0:/data/xash3d/engine.log` when debug mode is enabled.

Keep in mind that the game can hang for a long time when saving or autosaving (see issue [#7](https://github.com/fgsfdsfgs/vitaXash3D/issues/7)),
so don't kill it if it suddenly hangs when you enter a map or cross a checkpoint. You can disable autosaves in the "Game options" menu or with `sv_autosave 0`.

The mod launcher detects all directories you have inside `ux0:/data/xash3d/`, including `valve`, except directories that have names starting with a dot.

Installing mods works the same as with PC Half-Life: you just have to copy the mod directory (e. g. `darkstar`) into `ux0:/data/xash3d/`. Keep in mind
that if the mod comes with custom game DLLs (e. g. They Hunger, Counter-Strike, Opposing Force...), it most likely won't work correctly.

If you don't have the full version of HL and are using the Uplink archive linked above, multiplayer and other mods won't work. If you have the full version of HL, you can also install Uplink as a mod by copying **only** the `uplink` folder from the same archive.

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice;
- ArkSource for the Live Area stuff;
- everyone on the #henkaku and #vitasdk IRC channels for help and/or testing.
