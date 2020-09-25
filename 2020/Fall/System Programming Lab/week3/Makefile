# Makefile
CXX=gcc
CXXFLAGS=-w
TARGET=output
OBJECTS=main.o plus.o minus.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
main.o: main.c
	$(CXX) $(CXXFLAGS) -c $^
plus.o: plus.c
	$(CXX) $(CXXFLAGS) -c $^
minus.o: minus.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
