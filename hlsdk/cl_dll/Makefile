SRCS=../dlls/crossbow.cpp
SRCS+=../dlls/crowbar.cpp
SRCS+=../dlls/egon.cpp
SRCS+=./ev_hldm.cpp
SRCS+=../dlls/gauss.cpp
SRCS+=../dlls/handgrenade.cpp
SRCS+=./hl/hl_baseentity.cpp
SRCS+=./hl/hl_events.cpp
SRCS+=./hl/hl_objects.cpp
SRCS+=./hl/hl_weapons.cpp
SRCS+=../dlls/glock.cpp
SRCS+=../dlls/hornetgun.cpp
#SRCS+=../common/interface.cpp
SRCS+=../dlls/mp5.cpp
SRCS+=../dlls/python.cpp
SRCS+=../dlls/rpg.cpp
SRCS+=../dlls/satchel.cpp
SRCS+=../dlls/shotgun.cpp
SRCS+=../dlls/squeakgrenade.cpp
SRCS+=../dlls/tripmine.cpp
#SRCS+=../game_shared/voice_banmgr.cpp
#SRCS+=../game_shared/voice_status.cpp
SRCS+=./ammo.cpp
SRCS+=./ammo_secondary.cpp
SRCS+=./ammohistory.cpp
SRCS+=./battery.cpp
SRCS+=./cdll_int.cpp
SRCS+=./com_weapons.cpp
SRCS+=./death.cpp
SRCS+=./demo.cpp
SRCS+=./entity.cpp
SRCS+=./ev_common.cpp
SRCS+=./events.cpp
SRCS+=./flashlight.cpp
SRCS+=./GameStudioModelRenderer.cpp
SRCS+=./geiger.cpp
SRCS+=./health.cpp
SRCS+=./hud.cpp
SRCS+=./hud_msg.cpp
SRCS+=./hud_redraw.cpp
#SRCS+=./hud_servers.cpp
SRCS+=./hud_spectator.cpp
SRCS+=./hud_update.cpp
SRCS+=./in_camera.cpp
SRCS+=./input.cpp
SRCS+=./input_mouse.cpp
SRCS+=./input_goldsource.cpp
SRCS+=./menu.cpp
SRCS+=./message.cpp
SRCS+=./overview.cpp
SRCS+=./parsemsg.cpp
SRCS_C+=../pm_shared/pm_debug.c
SRCS_C+=../pm_shared/pm_math.c
SRCS_C+=../pm_shared/pm_shared.c
SRCS+=./saytext.cpp
SRCS+=./status_icons.cpp
SRCS+=./statusbar.cpp
SRCS+=./studio_util.cpp
SRCS+=./StudioModelRenderer.cpp
SRCS+=./text_message.cpp
SRCS+=./train.cpp
SRCS+=./tri.cpp
SRCS+=./util.cpp
SRCS+=./view.cpp
SRCS+=./input_xash3d.cpp
SRCS+=./scoreboard.cpp
SRCS+=./MOTD.cpp

OBJS = $(SRCS:.cpp=.o) $(SRCS_C:.c=.o)

PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
AR        = $(PREFIX)-ar

INCLUDES = -I../common -I. -I../game_shared -I../pm_shared -I../engine -I../dlls -I../utils/false_vgui/include -include "classhacks.h"
DEFINES = -Dstricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -DCLIENT_DLL
CFLAGS = -g -O2 -Wl,-q -Wfatal-errors -Wno-write-strings -fsigned-char -fno-lto $(DEFINES) $(INCLUDES)
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive
ASFLAGS = $(CFLAGS)

all: libclient.a

libclient.a: $(OBJS)
	$(AR) rcs $@ $^
	cp libclient.a ../../libclient.a

clean:
	@rm -rf libclient.a $(OBJS)
