
## if not define, define GAME
GAME ?= standard

RULESET_OBJECTS = extra.o map.o monsters.o rules.o world.o
##extra.o map.o monsters.o rules.o world.o

ENGINE_PATH = atlantis
ENGINE_OBJECTS = alist.o aregion.o army.o astring.o battle.o economy.o \
  edit.o faction.o fileio.o game.o gamedata.o gamedefs.o gameio.o \
  genrules.o i_rand.o items.o market.o modify.o monthorders.o \
  npc.o object.o orders.o parseorders.o production.o quests.o runorders.o \
  shields.o skills.o skillshows.o specials.o spells.o template.o unit.o

##OBJECTS = $(patsubst %.o,$(GAME)/obj/%.o,$(RULESET_OBJECTS)) \
##  $(patsubst %.o,obj/%.o,$(ENGINE_OBJECTS))
OBJECTS := $(patsubst %.o,obj/%.o,$(RULESET_OBJECTS)) \
          $(patsubst %.o,obj/%.o,$(ENGINE_OBJECTS)) \
	  obj/parse_gamedata.o

CXXFLAGS = -std=gnu++11 -g -I. -I..



## A directory to store objects
OBJDIR = obj
$(shell mkdir -p $(OBJDIR) >/dev/null)
## A directory to store the depency files
DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
## $(shell mkdir -p $(DEPDIR)/$(GAME) >/dev/null)

## Special GCC flags for the compilator to generate dependency files
##  -MT $@ : sets the name of the target
##  -MMD : generate as side effect of compilation
##  -MP : add target for eah prerequesite
##  -MF $(DEPDIR)/$*.Td : write generated dependency to temporary
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
## Rename temporary dependencies to dependencies
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

## Delete default built-in rule
%.o : %.c
## and add new rule where dependencies are needed
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cc
%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
obj/%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
obj/%.o : $(ENGINE_PATH)/$(GAME)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
obj/%.o : $(ENGINE_PATH)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
# $(GAME)/%.o: $(GAME)/%.cpp $(DEPDIR)/%.d
# 	$(COMPILE.cc) $(OUTPUT_OPTION) $<
# 	$(POSTCOMPILE)


all: parse_gamedata

parse_gamedata: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -o $@ $^
	@echo "INC="$(TO_INCLUDE)

worldedit: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -o $@ $^
	@echo "INC="$(TO_INCLUDE)


## Prevent pb if clean is a name of a file
## -rm : ignore errors in rm
.PHONY : clean
clean:
	-rm $(OBJECTS)
	-rm -rf $(DEPDIR)
	-rm -rf $(OBJDIR)

## Pattern rule with empty recipe so make will not fail if dependency file
## does not exists
$(DEPDIR)/%.d: ;
## Precious : will not be deleted as intermediate files
.PRECIOUS: $(DEPDIR)/%.d

## Include the dependency files that exists
## the '-include' avoid failing on non-existent
################################################################################
##                                                                            ##
## WARNING : at end of file                                                   ##
##                                                                            ##
################################################################################
TO_INCLUDE = $(patsubst %,$(DEPDIR)/%.d,$(basename $(RULESET_OBJECTS))) \
             $(patsubst %,$(DEPDIR)/%.d,$(basename $(ENGINE_OBJECTS))) \
	     $(DEPDIR)/map_editor.d

-include $(TO_INCLUDE)
################################################################################


