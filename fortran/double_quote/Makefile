TARGETS = double_quote

FC = gfortran
FFLAGS    = -std=legacy -fbackslash -Wno-argument-mismatch -fPIC

all: double_quote
bit_test: bit_test.o
	$(FC) double_quote.o -o double_quote

%.o: %.F
	$(FC) -I./ $(FFLAGS)   -c $<
clean:
	rm -f *.o *~ $(TARGETS)
