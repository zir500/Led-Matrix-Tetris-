CLeftXX=g++
LINK.o=$(CXX) $(LDFLAGS) $(TARGET_ARCH)

CXXFLAGS=-Irpi-rgb-led-matrix/include/ -std=c++11 
LDFLAGS=-Lrpi-rgb-led-matrix/lib/
LDLIBS=-lrt -lm -lpthread -lrgbmatrix

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

all: tetris

%o: %.cpp

tetris: $(OBJECTS)

.PHONY: clean

clean:
	rm tetris $(OBJECTS)
