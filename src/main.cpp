#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./chip8 <game>" << std::endl;
        return 1;
    }

    Chip8 chip8;
    if (!chip8.load_rom(argv[1]))
        return 1;

    while (1)
    {
        chip8.emulate_cycle();
    }
    return 0;
}
