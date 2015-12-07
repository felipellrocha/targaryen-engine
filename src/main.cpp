#include <SDL2/SDL.h>
#include <iostream>

#include "json.hpp"
#include "readjson.h"
#include "renderer.h"
#include "tile.h"

using json = nlohmann::json;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] ) {
  SDL_Window *win = nullptr;
  SDL_Renderer *ren = nullptr;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  // creating a window
  win = SDL_CreateWindow(
    "Game",
    100,
    100,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (win == nullptr) {
    std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // creating a renderer
  ren = SDL_CreateRenderer(
    win,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  json data = readFile("assets/pirates.json");
  //printf("%s", data.dump(4).c_str());
  printf("%s", data.at("orientation").get<string>().c_str());
  Renderer r;
  //TileLayer T(data, "foreground");

  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) break;

    r.render(ren);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
