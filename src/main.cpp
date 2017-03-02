#include <SDL2/SDL.h>
#include <emscripten/emscripten.h>
#include <iostream>

#include "timer.h"
#include "exceptions.h"
#include "renderer.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

LTimer fpsTimer;
LTimer capTimer;
int countedFrames = 0;

void loop(Renderer r) {
  capTimer.start();

  float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
  if (avgFPS > 2000000) avgFPS = 0;

  r.input();
  r.render();

  countedFrames++;

  int frameTicks = capTimer.getTicks();
  if (frameTicks < SCREEN_TICKS_PER_FRAME)
    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
}

extern "C" int mainf() {
  fpsTimer.start();
  Renderer *game = new Renderer("assets/pirates.json");

  emscripten_set_main_loop_arg((em_arg_callback_func)loop, game, -1, 1);

  SDL_Quit();
  return 0;
}
