INCLUDES = -I../common -I../engine -I../engine/common -I../engine/client -I../pm_shared

CFILES   := $(wildcard *.c)
CXXFILES := $(wildcard *.cpp)
OBJS     := $(CFILES:.c=.o) $(CXXFILES:.cpp=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
CFLAGS    = -g -nostartfiles -mtune=cortex-a9 -mfpu=neon -Ofast -Wl,-q -Wfatal-errors -fsigned-char -fno-lto -fno-short-enums \
            $(INCLUDES) \
            -DXASH_STATIC -DXASH_SINGLE_BINARY

CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS   = $(CFLAGS)

LIBS = -lc -lm -lSceNet_stub_weak -lSceNetCtl_stub_weak \
       -lSceSysmem_stub_weak -lSceSysmodule_stub_weak -lSceIofilemgr_stub_weak \
       -lSceLibKernel_stub_weak -lSceKernelThreadMgr_stub_weak -lSceKernelModulemgr_stub_weak

menu.suprx: menu.velf
	vita-make-fself -s $< $@

%.velf: %.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create -e $(<:.elf=.yml) $< $@

menu.elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(OBJS)
