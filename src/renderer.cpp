#include "renderer.h"


Renderer::Renderer(string levelFile) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL: " << SDL_GetError() << std::endl;
    SDL_Quit();
    throw renderer_error();
  }

  // creating a window
  this->win = SDL_CreateWindow(
    "Game",
    0, 0,
    1280, 480,
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
    SDL_RENDERER_ACCELERATED
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

  json level_data = readFile(levelFile.c_str());

  for (int i = 0; i < level_data.at("layers").size(); i++) {
    auto element = level_data.at("layers").at(i);
    string type = element
      .at("type")
      .get<string>();

    Node * layer = nullptr;
    if (type == "imagelayer") layer = new ImageLayer(this->ren, level_data, i);
    if (type == "tilelayer") layer = new TileLayer(this->ren, level_data, i);
    this->nodes.push_back(layer);
  }

  json character_data = readFile("assets/jean.json");

  Node * c = new CharacterLayer(this->ren, character_data);
  this->nodes.push_back(c);

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

void Renderer::input() {
}

