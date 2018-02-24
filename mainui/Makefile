INCLUDES = -I../common -I../engine -I../engine/common -I../engine/client -I../pm_shared

CFILES   := $(wildcard *.c)
CXXFILES := $(wildcard *.cpp)
OBJS     := $(CFILES:.c=.o) $(CXXFILES:.cpp=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
CFLAGS    = -g -mtune=cortex-a9 -mfpu=neon -Ofast -Wl,-q -Wfatal-errors -fsigned-char -fno-lto \
            $(INCLUDES) \
            -DXASH_STATIC -DXASH_SINGLE_BINARY

CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS   = $(CFLAGS)

all: libxashmenu.a

libxashmenu.a: $(OBJS)
	ar rcs $@ $^
	cp libxashmenu.a ../libxashmenu.a

clean:
	@rm -rf libxashmenu.a $(OBJS)
