CPPC=g++
CFLAGS=-lGLEW -lGL -lGLU -lglut
STD=-std=c++11
VERBOSE=-v

all:
	$(CPPC) $(STD) $(CFLAGS) *.cpp Classes/*.cpp -o trabCg

clean:
	rm -rf trabCg *.o *.dSYM



