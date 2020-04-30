TARGET    = roothb

ROOT_VER_GTEQ_6 = $(shell expr `root-config --version|cut -f1 -d/` \>= 6.0)
ifeq ($(ROOT_VER_GTEQ_6),1)
#root version >= 6
ROOTLIBS  = $(shell root-config --glibs) -lgfortran
ROOTCINT  = rootcling
else
# root version 5
ROOTLIBS  = -L$(shell root-config --libdir) -lCore -lCint -lRIO -lNet -lHist -lGraf -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lgfortran
ROOTCINT  = rootcint
endif

GCC_VER_GTEQ_6 = $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 6.0)
ifeq ($(GCC_VER_GTEQ_6),1)
PIEFLAGS  =  -O -no-pie
else
PIEFLAGS  =
endif

CC        = gcc
CXX       = g++
FC        = gfortran
CFLAGS    = $(PIEFLAGS)
CXXFLAGS  = $(PIEFLAGS) $(shell root-config --cflags)
FFLAGS    = $(PIEFLAGS) -std=legacy
LFLAGS    = $(PIEFLAGS)

all:	$(TARGET)

$(TARGET): $(TARGET).o h2root_funcDict.o h2root_func.o  MyTFileDict.o MyTFile.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	$(CXX)      $(LFLAGS) -o $@ $^ $(ROOTLIBS)
h2root_funcDict.cxx: h2root_func.h h2root_funcLinkDef.h
	$(ROOTCINT) -f $@ -c $^
h2root_funcDict.o: h2root_funcDict.cxx
	$(CXX) $(CXXFLAGS) -c $<
MyTFileDict.cxx: MyTFile.h MyTFileLinkDef.h
	$(ROOTCINT) -f $@ -c $^
MyTFileDict.o: MyTFileDict.cxx
	$(CXX) $(CXXFLAGS) -c $<
.cc.o:
	$(CXX) $(CXXFLAGS) -c $<
.c.o:
	$(CC)  $(CFLAGS)   -c $<
.f.o:
	$(FC)  $(FFLAGS)   -c $<
.PHONY : clean
clean:
	rm -rf *.o *Dict.cxx *Dict.h
