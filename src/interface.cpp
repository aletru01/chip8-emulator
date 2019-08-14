#include "interface.h"

#define WIDTH 1024
#define HEIGHT 512

Interface::Interface()
{
    if (SDL_INIT_EVERYTHING < 0)
        throw std::runtime_error("Error: SDL_Init");

    window = SDL_CreateWindow("chip8-emulator",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
        throw std::runtime_error("Error: SDL_CreateWindow");

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);

    if (!renderer)
        throw std::runtime_error("Error: SDL_CreateRenderer");

    keymap = { SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_q, SDLK_w, SDLK_e,
        SDLK_r, SDLK_a, SDLK_s, SDLK_d, SDLK_f, SDLK_z, SDLK_x, SDLK_c, SDLK_v,
    };
}

Interface::~Interface()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Interface::draw_screen(std::array<std::array<bool, 64>, 32>& screen)
{
    int width = WIDTH / 64;
    int height = HEIGHT / 32;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    for (int j = 0; j < (int)screen.size(); j++)
    {
        for (int i = 0; i < (int)screen[j].size(); i++)
        {
            if (screen[j][i])
            {
                SDL_Rect rect = {i * width, j * height, width, height};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

bool Interface::keyboard_handler(std::array<bool, 16>& keys)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return true;

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return true;

            for (std::size_t i = 0; i < keys.size(); i++)
                if (event.key.keysym.sym == keymap[i])
                    keys[i] = true;
        }

        if (event.type == SDL_KEYUP)
        {
            for (std::size_t i = 0; i < keys.size(); i++)
                if (event.key.keysym.sym == keymap[i])
                    keys[i] = false;
        }
    }
    return false;
}
