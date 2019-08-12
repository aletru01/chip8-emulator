CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Werror -g
LDLIBS = -lSDL2

VPATH = src

all: chip8

chip8: chip8.cpp main.cpp

clean:
	$(RM) chip8
