#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>

#include "timer/timer.h"
#include "renderer/renderer.h"
#include "exceptions.h"
#include "renderer/surrounding.h"
#include "entity/entity.h"
#include "game/components.h"
#include "game/systems.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

LTimer fpsTimer;
LTimer capTimer;
int countedFrames = 0;

void loop(Renderer &renderer, RenderSystem &rs) {
  capTimer.start();

  float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
  if (avgFPS > 2000000) avgFPS = 0;

  SDL_RenderClear(renderer.ren);

  renderer.input();
  renderer.physics();
  renderer.update();
  renderer.render();

  rs.update((double)countedFrames / fpsTimer.getTicks());

  SDL_RenderPresent(renderer.ren);

  countedFrames++;

  int frameTicks = capTimer.getTicks();
  if (frameTicks < SCREEN_TICKS_PER_FRAME)
    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
}

#ifdef __EMSCRIPTEN__
extern "C" int mainf() {
  fpsTimer.start();

  Renderer *game = new Renderer("assets/game.targ");

  emscripten_set_main_loop_arg((em_arg_callback_func)loop, game, -1, 1);

  delete game;

  SDL_Quit();
  return 0;
}
#else

int main() {
  fpsTimer.start();

  Renderer game = Renderer("assets/game.targ");

  EntityManager manager = EntityManager();

  Entity* camera = manager.createEntity();
  manager.addComponent<CameraComponent>(camera, new CameraComponent(0, 0));

  Entity* player = manager.createEntity();
  manager.addComponent<HealthComponent>(player, new HealthComponent(5, 5));
  manager.addComponent<PositionComponent>(player, new PositionComponent(48 * 2, 48 * 2));
  manager.addComponent<PositionComponent>(player, new PositionComponent(48 * 2, 48 * 2));
  manager.addComponent<SpriteComponent>(player, new SpriteComponent(0, 0, 48, 48, "character-1.png", game.ren));

  Entity* player2 = manager.createEntity();
  manager.addComponent<HealthComponent>(player2, new HealthComponent(5, 5));
  manager.addComponent<PositionComponent>(player2, new PositionComponent(0, 0));
  manager.addComponent<SpriteComponent>(player2, new SpriteComponent(0, 0, 48, 48, "character-2.png", game.ren));

  RenderSystem renderSystem = RenderSystem(manager, game.ren);

  while (game.isRunning()) {
    loop(game, renderSystem);
  }

  SDL_Quit();
  return 0;
}
#endif
