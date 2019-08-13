#include <iostream>
#include "chip8.h"
#include "graphics.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./chip8 <game>" << std::endl;
        return 1;
    }

    Chip8 chip8;
    Graphics graphics;

    if (!chip8.load_rom(argv[1]))
        return 1;

    while (1)
    {
        chip8.emulate_cycle();
        if (chip8.update_screen)
        {
            graphics.draw_screen(chip8.screen);
            chip8.update_screen = false;
        }

    }
    return 0;
}
