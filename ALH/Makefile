
CXX      = g++
CXXFLAGS = -g -O2
LDFLAGS  =  -L/usr/lib/python2.7/config
DEFS     = -DHAVE_CONFIG_H
LIBS     =  -lpython2.7 -L/usr/lib/x86_64-linux-gnu -pthread   -lwx_gtk2u_xrc-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_qa-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0 
CPPFLAGS =  -I/usr/include/python2.7 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-3.0 -I/usr/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread


#CC          = c++
#CXXFLAGS    = -O2  -Wall
#CXXFLAGS_WX = $(shell wx-config --cflags)
#LIBS_WX     = $(shell wx-config --libs)
#CXXFLAGS_PYTHON =-I/usr/include/python2.2 -DALH_PYTHON_EXTEND
#LIBS_PYTHON =-lpython2.2


OBJECTS     = collection.o cfgfile.o cstr.o files.o hash.o objs.o\
              data.o errs.o atlaparser.o \
              ahapp.o ahframe.o consts_ah.o editpane.o \
              editsframe.o extend.o extend_no.o \
			  flagsdlg.o hexfilterdlg.o listcoledit.o \
              listpane.o mapframe.o \
              mappane.o msgframe.o optionsdlg.o \
              unitfilterdlg.o unitframe.o unitframefltr.o unitpane.o \
              unitpanefltr.o unitsplitdlg.o utildlgs.o

TARGET      = ah


all: $(patsubst %.o,obj/%.o,$(OBJECTS))
	$(CXX) -s $(LDFLAGS) -o $(TARGET) $(patsubst %.o,obj/%.o,$(OBJECTS)) $(LIBS)

clean:
	rm -f $(patsubst %.o,obj/%.o,$(OBJECTS)) $(TARGET)

$(patsubst %.o,obj/%.o,$(OBJECTS)): obj/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $(DEFS) -o $@ $<
