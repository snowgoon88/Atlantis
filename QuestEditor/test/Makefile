
## The object files to be tested
APATH = ../..
BPATH = ..
TEST_OBJECTS = utils.o \
	item_data.o \
	monster_data.o \
	all_data.o \

ENGINE_OBJECTS = gamedata.o \

OBJECTS := $(patsubst %.o,obj/%.o,$(TEST_OBJECTS)) \
	$(patsubst %.o,obj/%.o,$(ENGINE_OBJECTS)) \


CXXFLAGS = -std=c++11 -g -I.. -I../..

## A directory to store objects
OBJDIR = obj
$(shell mkdir -p $(OBJDIR) >/dev/null)
## A directory to store the depency files
DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)

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
obj/%.o : $(APATH)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
obj/%.o : $(BPATH)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

all: test_enum

test_enum: $(OBJECTS) obj/test_enums.o
	@echo "__COMPILING $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -o $@ $^


## Prevent pb if clean is a name of a file
## -rm : ignore errors in rm
.PHONY : clean
clean:
	-rm -rf $(DEPDIR)
	-rm -rf $(OBJDIR)

.PHONY : veryclean
veryclean: clean
	-rm test_enum

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
