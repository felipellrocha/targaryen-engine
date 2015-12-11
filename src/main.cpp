#include <SDL2/SDL.h>
#include <iostream>

#include "timer.h"
#include "exceptions.h"
#include "renderer.h"
#include "tile.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main( int argc, char* args[] ) {
  SDL_Event event;
  LTimer fpsTimer;
  LTimer capTimer;
  int countedFrames = 0;
  fpsTimer.start();

  try {
    Renderer r("assets/pirates.json");
    //TileLayer T(data, "foreground");

    while (true) {
      capTimer.start();

      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) break;

      float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
      if (avgFPS > 2000000) avgFPS = 0;

      r.render();

      countedFrames++;

      int frameTicks = capTimer.getTicks();
      if (frameTicks < SCREEN_TICKS_PER_FRAME) SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
  } catch (renderer_error &stop) {
    return 1;
  }

  SDL_Quit();
  return 0;
}
