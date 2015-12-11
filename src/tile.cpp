#include "tile.h"

TileLayer::TileLayer(SDL_Renderer *renderer, json data, int layer) {

  this->renderer = renderer;

  this->tile_w = data.at("tilewidth").get<int>();
  this->tile_h = data.at("tileheight").get<int>();

  this->layer_count_w = data.at("width").get<int>();
  this->layer_count_h = data.at("height").get<int>();

  this->tile_count_w = data
    .at("tilesets")
    .at(0)
    .at("imagewidth")
    .get<int>() / this->tile_w;

  this->tile_count_h = data
    .at("tilesets")
    .at(0)
    .at("imageheight")
    .get<int>() / this->tile_h;

  string tileset_file = data
    .at("tilesets")
    .at(0)
    .at("image")
    .get<string>();

  string base_path = string(SDL_GetBasePath());
  string tileset_path = base_path + "assets/" + tileset_file;

  this->tiles = data.at("layers").at(layer).at("data").get<vector<int>>();

  this->texture = IMG_LoadTexture(this->renderer, tileset_path.c_str());

  if (this->texture == nullptr){
    std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    throw renderer_error();
  }

}

void TileLayer::render() {
  for (int i = 0; i < this->tiles.size(); i++) {
    int element = this->tiles[i];

    // Stupid dataset is not 0-based
    SDL_Rect src = {
      ((element - 1) % this->tile_count_w) * this->tile_w,
      ((element - 1) / this->tile_count_w) * this->tile_h,
      this->tile_w,
      this->tile_h
    };

    SDL_Rect dst = {
      (i % this->layer_count_w) * this->tile_w,
      (i / this->layer_count_w) * this->tile_h,
      this->tile_w,
      this->tile_h
    };

    SDL_RenderCopy(this->renderer, this->texture, &src, &dst);
  }
}
