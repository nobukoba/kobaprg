TARGET    = roothb
CXXFLAGS  = $(shell root-config --cflags)
CFLAGS    = -m64
FFLAGS    = -m64 -std=legacy
LFLAGS    = -m64
ROOTLIBS  = -L$(shell root-config --libdir) -lCore -lCint -lRIO -lNet -lHist -lGraf -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lgfortran

all:	$(TARGET)

$(TARGET): $(TARGET).o h2root_funcDict.o h2root_func.o  MyTFileDict.o MyTFile.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	g++      $(LFLAGS) -o $@ $^ $(ROOTLIBS)
h2root_funcDict.cxx: h2root_func.h h2root_funcLinkDef.h
	rootcint -f $@ -c $^
h2root_funcDict.o: h2root_funcDict.cxx
	g++      $(CXXFLAGS) -c $<
MyTFileDict.cxx: MyTFile.h MyTFileLinkDef.h
	rootcint -f $@ -c $^
MyTFileDict.o: MyTFileDict.cxx
	g++      $(CXXFLAGS) -c $<
.cc.o:
	g++      $(CXXFLAGS) -c $<
.c.o:
	gcc      $(CFLAGS)   -c $<
.f.o:
	gfortran $(FFLAGS)   -c $<
.PHONY : clean
clean:
	rm -rf *.o
