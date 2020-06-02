GCC_VER_GTEQ_5 = $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 5.0)
ifeq ($(GCC_VER_GTEQ_5),1)
PIEFLAGS  =  -O -no-pie
else
PIEFLAGS  =
endif

FC = gfortran $(PIEFLAGS)

CERNLIB = $(shell cernlib packlib)
#CERNLIB = -lgfortran -L$(HOME)/cern/cernlib_2005_minipacklib_hbook/2005/lib -lpacklib
# on miho-1
#CERNLIB = -lgfortran -L$(HOME)/cern/cernlib-2005-paw-128mb/2005/lib -lpacklib

all: htest
htest: htest.o
	$(FC) htest.o $(CERNLIB) -o htest
clean:
	rm -f *.o *~ htest
