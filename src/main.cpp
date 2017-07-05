#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "sdl2image.h"
#include <iostream>

#include "timer/timer.h"
#include "renderer/renderer.h"
#include "exceptions.h"
#include "entity/entity.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

LTimer fpsTimer;
LTimer capTimer;
int countedFrames = 0;


void loop(Renderer &renderer) {
  capTimer.start();

  float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
  if (avgFPS > 2000000) avgFPS = 0;

  SDL_RenderClear(renderer.ren);

  renderer.loop((double)countedFrames / fpsTimer.getTicks());

  SDL_RenderPresent(renderer.ren);

  countedFrames++;

  int frameTicks = capTimer.getTicks();
  if (frameTicks < SCREEN_TICKS_PER_FRAME) SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
}

#ifdef __EMSCRIPTEN__
extern "C" int mainf() {
  fpsTimer.start();

  EntityManager *manager = new EntityManager();
  Renderer game = Renderer("assets/rpg.targ", manager);

  emscripten_set_main_loop_arg((em_arg_callback_func)loop, &game, -1, 1);

  SDL_Quit();
  return 0;
}
#else

int main() {
  fpsTimer.start();

  EntityManager *manager = new EntityManager();
  Renderer game = Renderer("assets/rpg.targ", manager);

  while (game.isRunning()) {
    loop(game);
  }

  SDL_Quit();
  return 0;
}
#endif
