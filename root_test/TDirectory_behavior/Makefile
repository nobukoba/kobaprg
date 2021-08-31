TARGET    = TDirectory_behavior

CXX       = g++
CXXFLAGS  = $(shell root-config --cflags)
LFLAGS    = 
ROOTLIBS  = $(shell root-config --libs)

all:	$(TARGET)

$(TARGET): TDirectory_behavior.o
	$(CXX) $(LFLAGS) -o $@ $^ $(ROOTLIBS)
%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<
.PHONY : clean
clean:
	rm -rf $(TARGET) *.o *~
