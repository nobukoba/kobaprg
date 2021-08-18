ROOTINC = $(shell /home/kobayash/cern/root_v5.34.38_32bit_rpath/bin/root-config --cflags)
ROOTLIB = $(shell /home/kobayash/cern/root_v5.34.38_32bit_rpath/bin/root-config --libs)

CCFLAG   = -I. $(ROOTINC) $(ROOTLIB)
CC       = g++ -m32 -DLINUX $(CCENV) -D_FILE_OFFSET_BITS=64 -DMAC=0
cc       = gcc -m32 -DLINUX $(CCENV) -D_FILE_OFFSET_BITS=64 -DMAC=0


all:		
		make offline
# Nobu c/o 2021.03.22
#		make online
offline:	GEBSort_nogeb
GEBSort_nogeb:	GEBSort_nogeb.o bin_dgs.o
		$(CC) $^ -DHAVE_VXWORKS=0 $(ROOTLIB)  -o $@ -lz

GEBSort_nogeb.o:	GEBSort.cxx 
		$(CC) -DHAVE_VXWORKS=0 $(CCFLAG) $(ROOTINC)  $(CFLAGS) -c GEBSort.cxx -o GEBSort_nogeb.o

bin_dgs.o:	bin_dgs.c
		$(CC) $(CCFLAG) -c -o $@  bin_dgs.c

clean:		

		rm -f *.o
		rm -f GEBSort_nogeb
