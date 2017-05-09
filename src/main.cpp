#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
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
TTF_Font * Sans;
SDL_Surface * surfaceMessage;
SDL_Texture * Message;

void drawFPS(float fps, Renderer &renderer) {
	stringstream ss;
	ss << fixed << setprecision(2) << fps;

	string fpsString = ss.str();

  Sans = TTF_OpenFont("/Library/Fonts/Verdana.ttf", 14);

  SDL_Color White = {255, 255, 255};
	surfaceMessage = TTF_RenderText_Blended(Sans, fpsString.c_str(), White);

	Message = SDL_CreateTextureFromSurface(renderer.ren, surfaceMessage);

	SDL_Rect rect; //create a rect
  SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h );
	rect.x = 2;
	rect.y = 2;

	SDL_RenderCopy(renderer.ren, Message, NULL, &rect);
}

void loop(Renderer &renderer) {
  capTimer.start();

  float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
  if (avgFPS > 2000000) avgFPS = 0;

  SDL_RenderClear(renderer.ren);

  //renderer.input();
  //renderer.physics();
  //renderer.update();
  //renderer.render();

  renderer.loop((double)countedFrames / fpsTimer.getTicks());

  drawFPS(avgFPS, renderer);

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

  while (game.isRunning()) {
    loop(game);
  }

  SDL_Quit();
  return 0;
}
#endif
