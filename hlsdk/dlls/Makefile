PREFIX    = arm-vita-eabi
CC        = $(PREFIX)-gcc
CXX       = $(PREFIX)-g++
AR        = $(PREFIX)-ar

OBJS = \
	aflock.o \
	agrunt.o \
	airtank.o \
	animating.o \
	animation.o \
	apache.o \
	barnacle.o \
	barney.o \
	bigmomma.o \
	bloater.o \
	bmodels.o \
	bullsquid.o \
	buttons.o \
	cbase.o \
	client.o \
	combat.o \
	controller.o \
	crossbow.o \
	crowbar.o \
	defaultai.o \
	doors.o \
	effects.o \
	egon.o \
	explode.o \
	flyingmonster.o \
	func_break.o \
	func_tank.o \
	game.o \
	gamerules.o \
	gargantua.o \
	gauss.o \
	genericmonster.o \
	ggrenade.o \
	globals.o \
	gman.o \
	h_ai.o \
	h_battery.o \
	h_cine.o \
	h_cycler.o \
	h_export.o \
	handgrenade.o \
	hassassin.o \
	headcrab.o \
	healthkit.o \
	hgrunt.o \
	hornet.o \
	hornetgun.o \
	houndeye.o \
	ichthyosaur.o \
	islave.o \
	items.o \
	leech.o \
	lights.o \
	maprules.o \
	monstermaker.o \
	monsters.o \
	monsterstate.o \
	mortar.o \
	mp5.o \
	multiplay_gamerules.o \
	nihilanth.o \
	nodes.o \
	observer.o \
	osprey.o \
	pathcorner.o \
	plane.o \
	plats.o \
	player.o \
	playermonster.o \
	python.o \
	rat.o \
	roach.o \
	rpg.o \
	satchel.o \
	schedule.o \
	scientist.o \
	scripted.o \
	shotgun.o \
	singleplay_gamerules.o \
	skill.o \
	sound.o \
	soundent.o \
	spectator.o \
	squadmonster.o \
	squeakgrenade.o \
	subs.o \
	talkmonster.o \
	teamplay_gamerules.o \
	tempmonster.o \
	tentacle.o \
	triggers.o \
	tripmine.o \
	turret.o \
	util.o \
	weapons.o \
	world.o \
	xen.o \
	zombie.o \
	glock.o \
	../pm_shared/pm_debug.o \
	../pm_shared/pm_math.o \
	../pm_shared/pm_shared.o 

INCLUDES = -I. -I../engine -I../common -I../pm_shared -I../game_shared
DEFINES = -Dstricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -DCLIENT_WEAPONS -DNO_VOICEGAMEMGR
CFLAGS = -g -O2 -Wl,-q -Wfatal-errors -Wno-write-strings -fsigned-char -fno-lto $(DEFINES) $(INCLUDES)
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++11 -fpermissive -Wno-invalid-offsetof
ASFLAGS = $(CFLAGS)

all: libserver.a

libserver.a: $(OBJS)
	$(AR) rcs $@ $^
	cp libserver.a ../../libserver.a

clean:
	@rm -rf libserver.a $(OBJS)
