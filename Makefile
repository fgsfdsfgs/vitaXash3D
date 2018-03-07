APPNAME := vitaXash3D
TITLE := XASH00001

LIBS = -L. -lxashmenu -lclient -lserver -lvitaGL \
       -lSceLibKernel_stub -lSceAppMgr_stub -lSceSysmodule_stub \
       -lSceCtrl_stub -lSceTouch_stub -lm -lSceNet_stub -lSceNetCtl_stub \
       -lSceAppUtil_stub -lc -lScePower_stub -lSceCommonDialog_stub \
       -lSceAudio_stub -lSceGxm_stub -lSceDisplay_stub -lSceNet_stub -lSceNetCtl_stub

INCLUDES = -Iengine -Iengine/common -Icommon \
           -Iengine/server -Iengine/client \
           -Iengine/client/vgui -Ipm_shared

C_COMMON = $(wildcard engine/common/*.c) \
           $(wildcard engine/common/imagelib/*.c) \
           $(wildcard engine/common/soundlib/*.c) \
           $(wildcard engine/common/soundlib/libmpg/*.c)
C_CLIENT = $(wildcard engine/client/*.c)
C_SERVER = $(wildcard engine/server/*.c)
C_SYSTEM = $(wildcard engine/platform/vita/*.c)

CFILES   := $(C_SYSTEM) $(C_COMMON) $(C_SERVER) $(C_CLIENT)
# CPPFILES := $(CXX_MENU)
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
CFLAGS    = -g -mtune=cortex-a9 -mfpu=neon -Ofast -Wl,-q -Wfatal-errors -fsigned-char -fno-lto -fno-short-enums \
            $(INCLUDES) \
            -DXASH_STATIC -DXASH_SINGLE_BINARY \
            -DXASH_BUILD_COMMIT="\"$(shell git rev-parse --short HEAD 2> /dev/null || echo notset)\""

CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS   = $(CFLAGS)

all: launcher.bin libxashmenu.a libclient.a libserver.a $(APPNAME).vpk

launcher.bin:
	$(MAKE) -C modselector

libxashmenu.a:
	$(MAKE) -C mainui

libclient.a:
	$(MAKE) -C hlsdk/cl_dll clean
	$(MAKE) -C hlsdk/dlls clean
	$(MAKE) -C hlsdk/cl_dll

libserver.a:
	$(MAKE) -C hlsdk/cl_dll clean
	$(MAKE) -C hlsdk/dlls clean
	$(MAKE) -C hlsdk/dlls

shaders:
	$(MAKE) -C shaders

$(APPNAME).vpk: xash.velf launcher.bin
	vita-make-fself -s $< build/xash.bin
	cp -f launcher.bin build/eboot.bin
	vita-mksfoex -s TITLE_ID="$(TITLE)" -d ATTRIBUTE2=16 "$(APPNAME)" param.sfo
	cp -f param.sfo build/sce_sys/param.sfo
	
	vita-pack-vpk -s build/sce_sys/param.sfo -b build/eboot.bin \
		--add build/xash.bin=xash.bin \
		--add build/sce_sys/icon0.png=sce_sys/icon0.png \
		--add build/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add build/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add build/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		--add build/shaders/modulate_alpha_f.gxp=shaders/modulate_alpha_f.gxp \
		--add build/shaders/modulate_f.gxp=shaders/modulate_f.gxp \
		--add build/shaders/modulate_rgba_alpha_f.gxp=shaders/modulate_rgba_alpha_f.gxp \
		--add build/shaders/modulate_rgba_f.gxp=shaders/modulate_rgba_f.gxp \
		--add build/shaders/replace_alpha_f.gxp=shaders/replace_alpha_f.gxp \
		--add build/shaders/replace_f.gxp=shaders/replace_f.gxp \
		--add build/shaders/rgba_alpha_f.gxp=shaders/rgba_alpha_f.gxp \
		--add build/shaders/rgba_f.gxp=shaders/rgba_f.gxp \
		--add build/shaders/rgba_v.gxp=shaders/rgba_v.gxp \
		--add build/shaders/texture2d_rgba_v.gxp=shaders/texture2d_rgba_v.gxp \
		--add build/shaders/texture2d_v.gxp=shaders/texture2d_v.gxp \
		--add build/shaders/vertex_f.gxp=shaders/vertex_f.gxp \
		--add build/shaders/vertex_v.gxp=shaders/vertex_v.gxp \
		$(APPNAME).vpk

%.velf: %.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

xash.elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	$(MAKE) -C mainui clean
	$(MAKE) -C modselector clean
	$(MAKE) -C hlsdk/cl_dll clean
	$(MAKE) -C hlsdk/dlls clean
	@rm -rf libxashmenu.a libclient.a libserver.a launcher.bin
	@rm -rf build/eboot.bin build/xash.bin build/sce_sys/param.sfo param.sfo *.vpk.*
	@rm -rf xash.velf xash.elf $(OBJS)
