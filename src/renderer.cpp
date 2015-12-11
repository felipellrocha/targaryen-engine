#include "renderer.h"


Renderer::Renderer(string levelFile) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "SDL: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  // creating a window
  this->win = SDL_CreateWindow(
    "Game",
    0,
    0,
    1280,
    480,
    SDL_WINDOW_SHOWN
  );
  if (this->win == nullptr) {
    std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  // creating a renderer
  this->ren = SDL_CreateRenderer(
    this->win,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (this->ren == nullptr) {
    SDL_DestroyWindow(this->win);
    std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    std::cout << "IMG_Init Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  json data = readFile(levelFile.c_str());
  TileLayer *t1 = new TileLayer(this->ren, data, 4);
  TileLayer *t2 = new TileLayer(this->ren, data, 5);

  this->nodes.push_back(t1);
  this->nodes.push_back(t2);

};

Renderer::~Renderer() {
  for (int i = 0; i < this->nodes.size(); i++) delete this->nodes[i];
  SDL_DestroyRenderer(this->ren);
  SDL_DestroyWindow(this->win);
}

void Renderer::render() {

  SDL_RenderClear(this->ren);

  for (int i = 0; i < this->nodes.size(); i++) this->nodes[i]->render();

  SDL_RenderPresent(this->ren);
}
