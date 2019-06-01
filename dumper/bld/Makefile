CXX = g++

TARGET1 = first_event_dumper
TARGET2 = head_dumper
TARGET3 = bld_structure_analysis

all: $(TARGET1) $(TARGET2) $(TARGET3)
$(TARGET1): $(TARGET1).o
	$(CXX) $(TARGET1).o -o $(TARGET1)
$(TARGET2): $(TARGET2).o
	$(CXX) $(TARGET2).o -o $(TARGET2)
$(TARGET3): $(TARGET3).o
	$(CXX) $(TARGET3).o -o $(TARGET3)
clean:
	rm -f *.o *~
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
