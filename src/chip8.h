#ifndef CHIP8_H
# define CHIP8_H

#include <cstdint>
#include <array>

# define MEM_SIZE 4096

class Chip8
{
  public:
    Chip8();
    bool load_rom(char* filename);
    void clear_screen();
    void draw_sprite(int x, int y);
    bool key_pressed(int x);
    void emulate_cycle();

  private:
    std::array<std::uint8_t, MEM_SIZE> memory;
    std::array<std::uint8_t, 16> V;
    std::array<std::uint16_t, 16> stack;
    std::uint16_t sp;

    std::uint16_t I;
    std::uint16_t pc;
    std::uint16_t opcode;

    std::uint8_t delay_timer;
    std::uint8_t sound_timer;
    
    std::array<bool, 16> keys;

    std::array<std::array<bool, 64>, 32> screen;

    const std::array<std::uint8_t, 80> fontset =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    bool update_screen;
};

#endif
