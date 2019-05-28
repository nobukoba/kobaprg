CXX = g++

TARGET1 = first_event_dumper
TARGET2 = head_dumper

all: $(TARGET1) $(TARGET2)
$(TARGET1): $(TARGET1).o
	$(CXX) $(TARGET1).o -o $(TARGET1)
$(TARGET2): $(TARGET2).o
	$(CXX) $(TARGET2).o -o $(TARGET2)
clean:
	rm -f *.o *~
	rm -f $(TARGET1) $(TARGET2)
