#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "chip8.h"

Chip8::Chip8()
{
    opcode = 0;
    pc = 0x200;
    I = 0;
    sp = 0;

    delay_timer = 0;
    sound_timer = 0;

    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    keys.fill(false);

    for (std::size_t i = 0; i < fontset.size(); i++)
        memory[i] = fontset[i];

    clear_screen();
    update_screen = false;
    std::srand(std::time(nullptr));
}

bool Chip8::load_rom(char* filename)
{
    std::ifstream file(filename, std::ifstream::ate | std::ifstream::binary);
    auto size = file.tellg();

    if (size > MEM_SIZE - 0x200)
    {
        std::cerr << "File is too large" << std::endl;
        return false;
    }
    file.seekg(0, std::ifstream::beg);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    for (std::size_t i = 0; i < buffer.size(); i++)
        memory[i + 0x200] = buffer[i];

    return true;
}

void Chip8::clear_screen()
{
    for (auto &line : screen)
        line.fill(false);
}

void Chip8::draw_sprite(int x, int y)
{
    int height = opcode & 0x000F;
    int width = 8;
    V[0xF] = 0;
    std::uint8_t sprite;

    for (int j = 0; j < height; j++)
    {
        sprite = memory[I + j];
        for (int i = 0; i < width; i++)
        {
            if ((sprite & (0x80 >> i)) != 0)
            {
                if (screen[V[y] + i][V[x] + i] == 1)
                    V[0xF] = 1;
            }
            screen[V[y] + j][V[x] + i] = !screen[V[y] + j][V[x] + i];
        }
    }
    update_screen = true;
}

bool Chip8::key_pressed(int x)
{
    for (std::size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i])
        {
            V[x] = i;
            return true;
        }
    }
    return false;
}

void Chip8::emulate_cycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    auto x = (opcode & 0x0F00) >> 8;
    auto y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    clear_screen();
                    update_screen = true;
                    pc += 2;
                    break;

                case 0x000E:
                    pc = stack[--sp];
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode "
                        << std::hex << opcode << std::endl;
                    exit(1);
            }
            break;

        case 0x1000:
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:
            if (V[x] == (opcode & 0x00FF))
                pc += 2;
            pc += 2;
            break;

        case 0x4000:
            if (V[x] != (opcode & 0x00FF))
                pc += 2;
            pc += 2;
            break;

        case 0x5000:
            if (V[x] == V[y])
                pc += 2;
            pc += 2;
            break;

        case 0x6000:
            V[x] = opcode & 0x00FF;
            pc += 2;
            break;

        case 0x7000:
            V[x] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000:
                    V[x] = V[y];
                    pc += 2;
                    break;

                case 0x0001:
                    V[x] |= V[y];
                    pc += 2;
                    break;

                case 0x0002:
                    V[x] &= V[y];
                    pc += 2;
                    break;

                case 0x0003:
                    V[x] ^= V[y];
                    pc += 2;
                    break;

                case 0x0004:
                    V[0xF] = V[x] + V[y]> 0xFF ? 1 : 0;
                    V[x] += V[y];
                    pc += 2;
                    break;

                case 0x0005:
                    V[0xF] = V[x] < V[y] ? 0 : 1;
                    V[x] -= V[y];
                    pc += 2;
                    break;

                case 0x0006:
                    V[0xF] = V[x] & 0x1;
                    V[x] >>= 1;
                    pc += 2;
                    break;

                case 0x0007:
                    V[0xF] = V[x] > V[y] ? 0 : 1;
                    V[x] = V[y] - V[x];
                    pc += 2;
                    break;

                case 0x000E:
                    V[0xF] = V[x] >> 7;
                    V[x] <<= 1;
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode "
                        << std::hex << opcode << std::endl;
                    exit(1);
            }
            break;

        case 0x9000:
            if (V[x] != V[y])
                pc += 2;
            pc += 2;
            break;

        case 0xA000:
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000:
            pc = V[0x0] + (opcode & 0x0FFF);
            break;

        case 0xC000:
            V[x] = (std::rand() % 0x100) & (opcode & 0x00FF);
            pc += 2;
            break;

        case 0xD000:
            draw_sprite(x, y);
            pc += 2;
            break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E:
                    if (keys[V[x]] != 0)
                        pc += 2;
                    pc += 2;
                    break;

                case 0x00A1:
                    if (keys[V[x]] == 0)
                        pc += 2;
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode "
                        << std::hex << opcode << std::endl;
                    exit(1);
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    V[x] = delay_timer;
                    pc += 2;
                    break;

                case 0x000A:
                    if (key_pressed(x))
                        pc += 2;
                    break;

                case 0x0015:
                    delay_timer = V[x];
                    pc += 2;
                    break;

                case 0x0018:
                    sound_timer = V[x];
                    pc += 2;
                    break;

                case 0x001E:
                    V[0xF] = I + V[x] > 0xFFF ? 1 : 0;
                    I += V[x];
                    pc += 2;
                    break;

                case 0x0029:
                    I = V[x] * 5;
                    pc += 2;
                    break;

                case 0x0033:
                    memory[I] = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = V[x] % 10;
                    pc += 2;
                    break;

                case 0x0055:
                    for (int i = 0; i < x; i++)
                        memory[I + i] = V[i];
                    pc += 2;
                    break;

                case 0x0065:
                    for (int i = 0; i < x; i++)
                        V[I + i] = memory[i];
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode "
                        << std::hex << opcode << std::endl;
                    exit(1);
            }
            break;

        default:
            std::cerr << "Unknown opcode "
                << std::hex << opcode << std::endl;
            exit(1);
    }

    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0)
        sound_timer--;
}
