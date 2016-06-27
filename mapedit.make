# Makefile for MapEditor
#

GAME ?= standard

CPLUS = g++
CC = gcc
CFLAGS = -g -I. -I.. -Wall

RULESET_OBJECTS = extra.o map.o monsters.o rules.o world.o

ENGINE_OBJECTS = alist.o aregion.o army.o astring.o battle.o economy.o \
  edit.o faction.o fileio.o game.o gamedata.o gamedefs.o gameio.o \
  genrules.o i_rand.o items.o market.o modify.o monthorders.o \
  npc.o object.o orders.o parseorders.o production.o quests.o runorders.o \
  shields.o skills.o skillshows.o specials.o spells.o template.o unit.o

MAPEDITOR_OBJECTS = map_editor.o

OBJECTS = $(patsubst %.o,$(GAME)/obj/%.o,$(RULESET_OBJECTS)) \
  $(patsubst %.o,obj/%.o,$(ENGINE_OBJECTS)) \
  $(patsubst %.o,obj/%.o,$(MAPEDITOR_OBJECTS))

all: objdir $(OBJECTS)
	$(CPLUS) $(CFLAGS) -o mapedit $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	if [ -d obj ]; then rmdir obj; fi
	if [ -d $(GAME)/obj ]; then rmdir $(GAME)/obj; fi

FORCE:

objdir:
	if [ ! -d obj ]; then mkdir obj; fi
	if [ ! -d $(GAME)/obj ]; then mkdir $(GAME)/obj; fi


$(patsubst %.o,$(GAME)/obj/%.o,$(RULESET_OBJECTS)): $(GAME)/obj/%.o: $(GAME)/%.cpp
	$(CPLUS) $(CFLAGS) -c -o $@ $<

$(patsubst %.o,obj/%.o,$(ENGINE_OBJECTS)): obj/%.o: %.cpp
	$(CPLUS) $(CFLAGS) -c -o $@ $<

$(patsubst %.o,obj/%.o,$(MAPEDITOR_OBJECTS)): obj/%.o: %.cpp
	$(CPLUS) $(CFLAGS) -c -o $@ $<
