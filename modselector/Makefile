LIBS = -lvita2d -lSceAppMgr_stub -lSceDisplay_stub -lSceGxm_stub \
       -lSceSysmodule_stub -lSceCtrl_stub -lm \
       -lScePgf_stub -ljpeg -lfreetype -lc \
       -lSceCommonDialog_stub -lpng16 -lz

CFILES   := main.c utils.c
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
CFLAGS    = -g -mtune=cortex-a9 -mfpu=neon -Ofast -Wl,-q -Wfatal-errors -fsigned-char -fno-lto -fno-short-enums \
            -DXASH_BUILD_COMMIT="\"$(shell git rev-parse --short HEAD 2> /dev/null || echo notset)\""
CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS   = $(CFLAGS)

../launcher.bin: launcher.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< launcher.velf
	vita-make-fself -s launcher.velf ../launcher.bin

launcher.elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf launcher.velf launcher.elf $(OBJS)
