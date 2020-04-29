TARGETS   = s2root auto_s2root s2root_server
CC        = gcc -O -no-pie
CXX       = g++ -O -no-pie
FC        = gfortran -O -no-pie
CXXFLAGS  = $(shell root-config --cflags)
CFLAGS    = -m64
FFLAGS    = -m64 -std=legacy
LFLAGS    = -m64
ROOTLIBS  = $(shell root-config --libs) -lgfortran

all:	$(TARGETS)
s2root: s2root.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	$(CXX)     $(LFLAGS) -o $@ $^ $(ROOTLIBS)
auto_s2root: auto_s2root.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	$(CXX)     $(LFLAGS) -o $@ $^ $(ROOTLIBS)
s2root_server: s2root_server.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	$(CXX)     $(LFLAGS)  -o $@ $^ $(ROOTLIBS)
.cc.o:
	$(CXX)     $(CXXFLAGS) -c $<
.c.o:
	$(CC)      $(CFLAGS)   -c $<
.f.o:
	$(FC)      $(FFLAGS)   -c $<
.PHONY : clean
clean:
	rm -rf *.o
