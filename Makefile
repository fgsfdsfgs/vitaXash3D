TARGET := vitaXash3D
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

# GAME =

CFILES   := $(C_SYSTEM) $(C_COMMON) $(C_SERVER) $(C_CLIENT)
# CPPFILES := $(CXX_MENU)
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
CFLAGS    = -g -O2 -Wl,-q -Wfatal-errors -fsigned-char -fno-lto \
            $(INCLUDES) \
            -DXASH_STATIC -DXASH_SINGLE_BINARY

CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS   = $(CFLAGS)

all: libxashmenu.a libclient.a libserver.a $(TARGET).vpk

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

$(TARGET).vpk: $(TARGET).velf
	vita-make-fself -s $< build/eboot.bin
	vita-mksfoex -s TITLE_ID="$(TITLE)" "$(TARGET)" param.sfo
	cp -f param.sfo build/sce_sys/param.sfo
	
	vita-pack-vpk -s build/sce_sys/param.sfo -b build/eboot.bin \
		--add build/sce_sys/icon0.png=sce_sys/icon0.png \
		--add build/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add build/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add build/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		$(TARGET).vpk

%.velf: %.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@
	vita-make-fself -s $@ eboot.bin

$(TARGET).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	$(MAKE) -C mainui clean
	$(MAKE) -C hlsdk/cl_dll clean
	$(MAKE) -C hlsdk/dlls clean
	@rm -rf libxashmenu.a libclient.a libserver.a
	@rm -rf build/eboot.bin build/sce_sys/param.sfo param.sfo *.vpk.*
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
