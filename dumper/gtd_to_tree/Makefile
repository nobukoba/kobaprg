CXX = g++
CXXFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)

PROGRAM1 = gtd_to_tree
PROGRAM2 = gtd_tree_sort

all: $(PROGRAM1)  $(PROGRAM2)
$(PROGRAM1): $(PROGRAM1).o
	$(CXX) $(CXXFLAGS) -o $(PROGRAM1) $(PROGRAM1).o $(ROOTLIBS)
$(PROGRAM2): $(PROGRAM2).o
	$(CXX) $(CXXFLAGS) -o $(PROGRAM2) $(PROGRAM2).o $(ROOTLIBS)
clean:
	rm -f *.o *~
	rm -f $(PROGRAM1) $(PROGRAM2)
