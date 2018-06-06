# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This port is still a work in progress. Expect graphics and sound glitches, crashes and gameplay oddities.

## Requirements
- Vita SDK Toolchain (use [VDPM](https://github.com/vitasdk/vdpm) for installing)
- Latest version of [vitaGL](https://github.com/Rinnegatamante/vitaGL).
- [MSYS2](https://www.msys2.org) (only for building under Windows)

## Building on Windows
For building on Windows, you should to use [MSYS2](https://www.msys2.org).
- Install it, set `MSYS2_DIR` environment variable in Windows as path to msys2 directory (for example, C:\msys64).
- Add line `VITAXASH3D_DIR=*your path to cloned repo* # (as example, "C:\vitaXash3D")` to bottom of your `%MSYS2_DIR%/.bashrc` file.
- Using MSYS2 shell, do clone [vitaGL](https://github.com/Rinnegatamante/vitaGL) repo and set current directory to it, after run `make`.
- Place resulting `libvitaGL.a` file to `%VITASDK%/arm-vita-eabi/lib`. Also, place vitaGL header to `%VITASDK/arm-vita-eabi/include%`.
- After, build project using Visual Studio, or run MSYS2 shell, set current directory (`cd $VITAXASH3D_DIR`), and then run `make`.
This should produce a VPK file.

## Running
1. Copy the desired game directory (e.g. `valve` for Half-Life) to `ux0:/data/xash3d/`.
2. Obtain appropriate game libraries (`cl_dlls/client.suprx` and `dlls/server.suprx`) and copy them to the game directory. See below for list of game libraries ported to the Vita.
3. Copy the `build/data` directory to `ux0:/` on your Vita. Replace everything if asked.
4. Install the VPK obtained after building the port.
5. Run it.

#### Notes
The console log is saved to `ux0:/data/xash3d/engine.log` when debug mode is enabled.

If you don't have any GoldSrc games, you can use the data files from Half-Life Uplink, HL's free demo version.
To do so, just extract [this archive](https://drive.google.com/file/d/1Jzaz3cTroTK3--QYdhUF7D_7WfQOxTFj/view) to `ux0:/` **instead of doing step 1** from the guide above.

Keep in mind that the game can hang for a long time when saving or autosaving (see issue [#7](https://github.com/fgsfdsfgs/vitaXash3D/issues/7)),
so don't kill it if it suddenly hangs when you enter a map or cross a checkpoint. You can disable autosaves in the "Game options" menu or with `sv_autosave 0`.

Multiplayer seems to work, but it hasn't been tested much. You can connect to any Xash3D server as long as it is compatible with this port, meaning it has to be running the same mod as you on Xash3D-FWGS v0.19 or Xash3D build 3366.

The mod launcher detects all directories you have inside `ux0:/data/xash3d/`, including `valve`, except `launcher` and directories that have names starting with a dot.

Installing mods works the same as with PC Half-Life: you just have to copy the mod directory (e. g. `darkstar`) into `ux0:/data/xash3d/`. Keep in mind
that you'll also need Vita ports of the mod's game libraries, if it has any. For available ports, see list below.

If you're trying to run a mod that has no game libraries, Xash will try to fall back to the HL ones, so it's best that you have them installed at all times.

If you don't have the full version of HL and are using the Uplink archive linked above, multiplayer and other mods won't work. If you have the full version of HL, you can also install Uplink as a mod by copying **only** the `uplink` folder from the same archive.

If you're trying to connect to an online server from the server list and you get dropped back to main menu, then either you're banned on that server or it took too long to connect, in which case you just have to retry.

Some Xash servers automatically ban people connecting from a device other than an Android or iOS smartphone, nothing can be done about that.

Changing the video mode (screen resolution) immediately restarts the game. You can also change it by editing your mod's `video.cfg`.
Keep in mind that some mods, like Counter-Strike, were not meant to be played in a resolution smaller than 640x400.

To change your name either enter `name my_name` in the console or change `name` in your mod's `config.cfg`.

You can change the layout of on-screen touch buttons by using the "Editor" function in the "Touch buttons" menu.

Expect every new release to invalidate at least some of your old save files. Pretty much nothing can be done about this right now, as saves are function offset based, and those offsets change with almost every code change.

#### Available game libraries
* [Half-Life](https://github.com/fgsfdsfgs/hlsdk-xash3d) (included in vitaXash3D releases after v2)
* [Blue Shift](https://github.com/fgsfdsfgs/hlsdk-xash3d/tree/bshift) (included in vitaXash3D releases after v2)
* [Opposing Force](https://github.com/fgsfdsfgs/hlsdk-xash3d/tree/opfor) (included in vitaXash3D releases after v2)
* [Counter-Strike client](https://github.com/fgsfdsfgs/cs16-client/tree/v1.32) (included in vitaXash3D releases after v3)
* [Counter-Strike server](https://github.com/fgsfdsfgs/regamelite) (ReGameLite, includes ZBot, unstable)

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS and game DLL source code;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice;
- ArkSource for the Live Area stuff;
- KINGGOLDrus for some launcher graphics;
- everyone on the #henkaku and #vitasdk IRC channels for help and/or testing.
