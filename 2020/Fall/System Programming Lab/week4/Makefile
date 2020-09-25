# Makefile
CXX=gcc
CXXFLAGS=-w
TARGET=ex4
OBJECTS=ex4.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
ex4.o: ex4.c
	$(CXX) $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
