TARGETS = include_test

FC = gfortran
FFLAGS    = -std=legacy -Wno-argument-mismatch -fPIC

all: include_test
include_test: include_test.o
	$(FC) include_test.o -o include_test

%.o: %.F
	$(FC) -I./ $(FFLAGS)   -c $<
clean:
	rm -f *.o *~ $(TARGETS)
