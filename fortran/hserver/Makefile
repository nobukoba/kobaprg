FC = gfortran

#CERNLIB = $(shell cernlib packlib)
# on miho-1
CERNLIB = -lgfortran -L/home/tamidaq/cern/cernlib-2005-paw-128mb/2005/lib -lpacklib

all: hserver
hserver: hserver.o
	$(FC) hserver.o $(CERNLIB) -o hserver
clean:
	rm -f *.o *~
	rm -f hserver
