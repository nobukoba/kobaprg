CXX = g++
CXXFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)

PROGRAM = gtd_to_tree gtd_to_tree_w_trace gtd_tree_sort draw_traces

all: $(PROGRAM)
%: %.cc
	$(CXX) $(CXXFLAGS) -c $@.cc -o $@.o
	$(CXX) $(CXXFLAGS) $(ROOTLIBS) $@.o -o $@
clean:
	rm -f *.o *~
	rm -f $(PROGRAM)
