#include "imagelayer.h"

ImageLayer::ImageLayer(SDL_Renderer *renderer, json data, int layer) {
  this->renderer = renderer;

  this->x = data
    .at("layers")
    .at(layer)
    .at("x")
    .get<int>();

  this->y = data
    .at("layers")
    .at(layer)
    .at("y")
    .get<int>();

  try {
    this->tiling = data
      .at("layers")
      .at(layer)
      .at("properties")
      .at("tiling")
      .get<string>();
  } catch (const out_of_range &err) {
    this->tiling = "none";
  }

  string tileset_file = data
    .at("layers")
    .at(layer)
    .at("image")
    .get<string>();

  cout
    << this->tiling
    << ' '
    << this->x
    << ' '
    << this->y
    << ' '
    << endl;

  string base_path = string(SDL_GetBasePath());
  string tileset_path = base_path + "assets/" + tileset_file;

  cout << tileset_path << endl;

  this->texture = IMG_LoadTexture(this->renderer, tileset_path.c_str());
  if (this->texture == nullptr){
    std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    throw renderer_error();
  }

  SDL_QueryTexture(this->texture, NULL, NULL, &this->w, &this->h);

}

void ImageLayer::render() {
  if (this->tiling == "none") {
    SDL_Rect pos = {
      this->x, this->y, this->w, this->h
    };
    SDL_RenderCopy(this->renderer, this->texture, NULL, &pos);
  }
  if (this->tiling == "horizontal") {
    SDL_Rect pos = {
      this->x, this->y, this->w, this->h
    };
    SDL_RenderCopy(this->renderer, this->texture, NULL, &pos);
  }
}
