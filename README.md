# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This port is still a work in progress. Expect graphics and sound glitches, crashes and gameplay oddities.

## Requirements
- VitaSDK (in PATH and with VITASDK set);
- Latest version of [vitaGL](https://github.com/Rinnegatamante/vitaGL).

## Building
Run `make` in this directory. This should produce a VPK.

## Running
1. Copy the desired game directory (e.g. `valve` for Half-Life) to `ux0:/data/xash3d/`.
2. Obtain appropriate game libraries (`cl_dlls/client.suprx` and `dlls/server.suprx`) and copy them to the game directory. See below for list of game libraries ported to the Vita.
3. Copy the `build/data` directory to `ux0:/` on your Vita. Replace everything if asked.
4. Install the VPK obtained after building the port.
5. Run it.

#### Notes
The console log is saved to `ux0:/data/xash3d/engine.log` when debug mode is enabled.

If you don't have any GoldSrc games, you can use the data files from Half-Life Uplink, HL's free demo version.
To do so, just extract [this archive](https://drive.google.com/file/d/18DCj4KFS7GMaBZyQ3BPgdM8s1mA7AWED/view) to `ux0:/` **instead of doing step 1** from the guide above.

Keep in mind that the game can hang for a long time when saving or autosaving (see issue [#7](https://github.com/fgsfdsfgs/vitaXash3D/issues/7)),
so don't kill it if it suddenly hangs when you enter a map or cross a checkpoint. You can disable autosaves in the "Game options" menu or with `sv_autosave 0`.

Multiplayer seems to work, but it hasn't been tested much. You can connect to any Xash3D server as long as it is compatible with this port, meaning it has to be running the same mod as you on Xash3D-FWGS v0.19 or Xash3D build 3366.

The mod launcher detects all directories you have inside `ux0:/data/xash3d/`, including `valve`, except directories that have names starting with a dot.

Installing mods works the same as with PC Half-Life: you just have to copy the mod directory (e. g. `darkstar`) into `ux0:/data/xash3d/`. Keep in mind
that you'll also need Vita ports of the mod's game libraries, if it has any. For available ports, see list below.

If you're trying to run a mod that has no game libraries, Xash will try to fall back to the HL ones, so it's best that you have them installed at all times.

If you don't have the full version of HL and are using the Uplink archive linked above, multiplayer and other mods won't work. If you have the full version of HL, you can also install Uplink as a mod by copying **only** the `uplink` folder from the same archive.

#### Available game libraries
* [Half-Life](https://github.com/fgsfdsfgs/hlsdk-xash3d) (included in vitaXash3D releases after v2)
* [Blue Shift](https://github.com/fgsfdsfgs/hlsdk-xash3d/tree/bshift) (included in vitaXash3D releases after v2)
* [Opposing Force](https://github.com/fgsfdsfgs/hlsdk-xash3d/tree/opfor) (included in vitaXash3D releases after v2)

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS and game DLL source code;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice;
- ArkSource for the Live Area stuff;
- everyone on the #henkaku and #vitasdk IRC channels for help and/or testing.
