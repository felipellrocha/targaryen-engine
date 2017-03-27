#include "itemlayer.h"

ItemLayer::ItemLayer(SDL_Renderer *renderer, json data, int index, int tileType, int layer) {

  this->renderer = renderer;

  string tileset_file = data
    .at("tilesets")
    .at(0)
    .at("image")
    .get<string>();

  string base_path = string(SDL_GetBasePath());
  string tileset_path = base_path + "assets/" + tileset_file;

  this->texture = IMG_LoadTexture(this->renderer, tileset_path.c_str());

  if (this->texture == nullptr){
    std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    throw renderer_error();
  }

  this->w = data.at("tilewidth").get<int>();
  this->h = data.at("tileheight").get<int>();

  this->location_x = (index % data
    .at("layers")
    .at(layer)
    .at("width")
    .get<int>()) * this->w;

  this->location_y = (index / data
    .at("layers")
    .at(layer)
    .at("width")
    .get<int>()) * this->h;

  this->tile_x = ((tileType - 1) % data
    .at("tilesets")
    .at(0)
    .at("columns")
    .get<int>()) * this->w;

  this->tile_y = ((tileType - 1) / data
    .at("tilesets")
    .at(0)
    .at("columns")
    .get<int>()) * this->h;

  // uint is necessary here simply to disambiguate
  // the constructor call
  this->aabb = AABB(
    (uint)this->location_x,
    (uint)this->location_y,
    0,
    this->location_x + this->w,
    this->location_y + this->h,
    0
  );
}

void ItemLayer::render() {
  SDL_Rect src = {
    this->tile_x,
    this->tile_y,
    this->w,
    this->h
  };

  SDL_Rect dst = {
    this->location_x,
    this->location_y,
    this->w,
    this->h
  };

  SDL_RenderCopy(this->renderer, this->texture, &src, &dst);
}

void ItemLayer::input(SDL_Event event) {
}

void ItemLayer::update() {
}


