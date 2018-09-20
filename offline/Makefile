CXX = g++
CXXFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)

PROGRAM = offline 

all: $(PROGRAM)
offline: offline.o event1.o command.h
	$(CXX) $(CXXFLAGS) -o offline offline.o event1.o $(ROOTLIBS)
clean:
	rm -f *.o *~
	rm -f $(PROGRAM)
