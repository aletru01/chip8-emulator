#pragma once

#include <SDL2/SDL.h>
#include <array>

class Interface
{
  public:

      Interface();
      ~Interface();
      void draw_screen(std::array<std::array<bool, 64>, 32>& screen);
      bool keyboard_handler(std::array<bool, 16>& keys);
  private:

      SDL_Window* window;
      SDL_Renderer* renderer;
      std::array<std::uint8_t, 16> keymap;
};
