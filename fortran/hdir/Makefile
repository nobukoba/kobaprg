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
#CERNLIB = -L$(HOME)/cern/cernlib_2005_minipacklib_hbook_sandbox/2005/lib -lpacklib -lgfortran 
CERNLIB = -L$(HOME)/cern/cernlib_2005_minipacklib_hbook/2005/lib -lpacklib -lgfortran 

all: hdir
hdir: hdir.o
	$(FC) hdir.o $(CERNLIB) -o hdir
clean:
	rm -f *.o *~
	rm -f hdir
