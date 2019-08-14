CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic
LDLIBS = -lSDL2

VPATH = src

all: chip8

chip8: chip8.cpp main.cpp interface.cpp

clean:
	$(RM) chip8
