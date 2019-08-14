#include <iostream>
#include "chip8.h"
#include "interface.h"
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./chip8 <game>" << std::endl;
        return 1;
    }

    Chip8 chip8;
    Interface interface;

    if (!chip8.load_rom(argv[1]))
        return 1;

    while (true)
    {
        chip8.emulate_cycle();

        if (chip8.update_screen)
        {
            interface.draw_screen(chip8.screen);
            chip8.update_screen = false;
        }

        if (interface.keyboard_handler(chip8.keys))
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
