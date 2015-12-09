#include <SDL2/SDL.h>
#include <iostream>

#include "renderer.h"
#include "tile.h"

int main( int argc, char* args[] ) {
  SDL_Event event;

  try {
    Renderer r("assets/pirates.json");
    //TileLayer T(data, "foreground");

    while (true) {
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) break;

      r.render();
    }
  } catch (renderer_error &stop) {
    return 1;
  }

  SDL_Quit();
  return 0;
}
