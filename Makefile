CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -g
LDLIBS = -lSDL2

VPATH = src

all: chip8

chip8: chip8.cpp main.cpp graphics.cpp

clean:
	$(RM) chip8
