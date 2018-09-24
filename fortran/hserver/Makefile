FC = gfortran
CERNLIB = $(shell cernlib packlib)

all: hserver
hserver: hserver.o
	$(FC) hserver.o $(CERNLIB) -o hserver
clean:
	rm -f *.o *~
	rm -f hserver
