#include "graphics.h"

#define WIDTH 1024
#define HEIGHT 512


Graphics::Graphics()
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
}

Graphics::~Graphics()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::draw_screen(std::array<std::array<bool, 64>, 32>& screen)
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
