TARGETS   = shm_display

GCC_VER_GTEQ_5 = $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 5.0)
ifeq ($(GCC_VER_GTEQ_5),1)
PIEFLAGS  =  -O -no-pie
else
PIEFLAGS  =
endif

CC        = gcc $(PIEFLAGS)
CXX       = g++ $(PIEFLAGS)
FC        = gfortran $(PIEFLAGS)
CFLAGS    = 
CXXFLAGS  = $(shell root-config --cflags)
FFLAGS    = -std=legacy
LFLAGS    = 
ROOTLIBS  = $(shell root-config --libs) -lRHTTP -lgfortran

all:	$(TARGETS)
shm_display: shm_display.o hlimap.o hidall.o mzwork.o hcreatem.o hshm.o hmapm.o hrin2.o hcopyu.o hcopyn.o hcopyt.o zebra.o hbook.o cernlib.o kernlib.o
	$(CXX)     $(LFLAGS) -o $@ $^ $(ROOTLIBS)
.cc.o:
	$(CXX)     $(CXXFLAGS) -c $<
.c.o:
	$(CC)      $(CFLAGS)   -c $<
.f.o:
	$(FC)      $(FFLAGS)   -c $<
.PHONY : clean
clean:
	rm -rf *.o
