#pragma once

#include <SDL2/SDL.h>
#include <array>

class Graphics
{
  public:

      Graphics();
      ~Graphics();
      void draw_screen(std::array<std::array<bool, 64>, 32>& screen);
  private:

      SDL_Window* window;
      SDL_Renderer* renderer;
};
