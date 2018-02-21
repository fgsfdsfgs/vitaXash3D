# vitaXash3D
A port of Xash3D-FWGS to the PSVita.

## Warning
This is nowhere near playable yet. The only thing that works more or less correctly right now is the menu system and the console.
HLSDK is statically linked into the engine executable, so you can only use this with Half-Life right now.
Any other GoldSrc mod/game would require some modifications to be able to statically link in the same way, I'll write a guide on that later.

## Requirements
- VitaSDK (in PATH and with VITASDK set);
- [vitaGL](https://github.com/Rinnegatamante/vitaGL).

## Building
Run `make` in this directory. This should produce a VPK.

## Running
1. Copy the `build/data` directory to `ux0:/` on your Vita.
2. Copy the `valve` directory from your Half-Life installation (preferably the Steam version) to `ux0:/data/xash3d/` on your Vita.
3. Delete everything from `valve/cl_dlls` and `valve/dlls`.
4. Install the VPK obtained after building the port.
5. Run it. The console log is saved to `ux0:/data/xash3d/engine.log`.

## Credits
- Uncle Mike & co for Xash3D and FWGS team for Xash3D-FWGS;
- Vita SDK Team for the Vita SDK;
- Rinnegatamante for vitaGL, vitaQuake/vitaQuakeII and general advice.
