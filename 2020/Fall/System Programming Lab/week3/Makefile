# Makefile
CXX=gcc
TARGET=output
OBJECTS=main.o plus.o minus.o

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^
main.o: main.c
	$(CXX) -c $^
plus.o: plus.c
	$(CXX) -c $^
minus.o: minus.c
	$(CXX) -c $^
clean:
	rm $(OBJECTS) $(TARGET)
