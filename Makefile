CPPC=g++
CFLAGS=-lGLEW -lGL -lGLU -lglut -g
STD=-std=c++11
VERBOSE=-v

all:
	$(CPPC) $(STD) -o trabCG -I. -I./Classes *.cpp ./Classes/*.cpp $(CFLAGS) 

clean:
	rm -rf trabCg *.o *.dSYM



