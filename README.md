# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This is currently very broken and not user-friendly at all. Expect severe graphics glitches, crashes and gameplay oddities.

HLSDK is statically linked into the engine executable, so you can only use this with Half-Life right now.

Any other GoldSrc mod/game would require some modifications to be able to hardlink in the same way, I'll write a guide on that later.

## Status/Known issues
You can create issues for anything that is not listed here.
* Graphics:
  * skybox textures render as the "missing texture" texture;
  * dynamic lights cause artifacts in lightmaps, esp. if the dlight hits a transparent surface;
  * model lighting is somewhat broken: there's no normals or glShadeModel;
  * if the engine fails to load a texture for a decal or sprite it will infinitely retry loading it, lagging the game to hell;
  * some translucent textures render as opaque;
  * translucent HUD elements have "holes" in places with high color intensity;
  * some particles and tracers don't render at all or (in case of MP5 tracers) point towards origin;
  * func_breakable gibs either don't render or don't spawn at all (HLSDK related?);
  * guns produce no bullet impact decals, sparks, brass or tracers (HLSDK related?);
  * BMP menu elements (`gfx/shell/btns_main.bmp` & co in WON HL's `pak0.pak` or in the unpatched `extras.pak`) don't render at all, so they're disabled for now;
  * rendering is slow, especially for models.
* Sound:
  * crackling and other shit happens whenever the game hangs/loads something;
  * the internal mixer uses 44KHz sample rate, while snd_vita uses 48KHz, making sound a bit too high pitched.
* Input:
  * Vita buttons map to keyboard keys, analog sticks are actually handled like joysticks;
  * button->key and joystick axis mappings are hardcoded;
  * deadzones are hardcoded and there's no analog rescaling;
  * no way to input text in the console;
  * no touch controls for anything.
* Game:
  * saving/loading and map transitions via changelevel need extensive testing;
  * cl_lw breaks a lot of weapons, so keep it disabled for now;
  * func_pushables slide endlessly after you push them as if there's no friction;
  * HLSDK is statically linked into the engine executable, so there's no easy way to change/port mods.
* Misc:
  * code quality is absolute ass, there's ifdefs everywhere, will have to either remove them and make a standalone port or add Vita as a platform properly;
  * heap size might be too small, but increasing it to 192 makes it crash on startup;
  * file I/O is slow as shit, investigate;
  * networking is broken and currently is not a priority.

## Requirements
- VitaSDK (in PATH and with VITASDK set);
- [vitaGL](https://github.com/Rinnegatamante/vitaGL).

## Building
Run `make` in this directory. This should produce a VPK.

## Running
1. Copy the `valve` directory from your Half-Life installation (preferably the Steam version) to `ux0:/data/xash3d/` on your Vita.
2. Copy the `build/data` directory to `ux0:/` on your Vita. Replace everything if asked.
3. Delete everything from `valve/cl_dlls` and `valve/dlls`.
4. Install the VPK obtained after building the port.
5. Run it. The console log is saved to `ux0:/data/xash3d/engine.log`.

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice.
