GCC_VER_GTEQ_5 = $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 5.0)
ifeq ($(GCC_VER_GTEQ_5),1)
PIEFLAGS  =  -O -no-pie
else
PIEFLAGS  =
endif

FC = gfortran $(PIEFLAGS)

#CERNLIB = $(shell cernlib packlib)
# on miho-1
#CERNLIB = -lgfortran -L/home/tamidaq/cern/cernlib-2005-paw-128mb/2005/lib -lpacklib
#CERNLIB = -lgfortran -L/home/kobayash/cern/cernlib-2005/2005/lib -lpacklib

#CERNLIB = -lgfortran -L/home/kobayash/cern/cernlib_2005_minipacklib_hbook/2005/lib -lpacklib
CERNLIB = -lgfortran -Wl,-Bstatic -L/saho/sharedAPL/cernlib/usr/lib64/cernlib/2006/lib -lpacklib -lkernlib  -Wl,-Bdynamic

all: hserver mebae_server
hserver: hserver.o
	$(FC) hserver.o $(CERNLIB) -o hserver
mebae_server: mebae_server.o
	$(FC) mebae_server.o $(CERNLIB) -o mebae_server
clean:
	rm -f *.o *~
	rm -f hserver mebae_server
