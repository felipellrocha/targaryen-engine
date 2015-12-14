#include "characterlayer.h"

CharacterLayer::CharacterLayer(SDL_Renderer *renderer, json data) {
  this->renderer = renderer;
  this->data = data;

  this->state = new IdlingState(this);

  string tileset_file = data.at("image").get<string>();

  string base_path = string(SDL_GetBasePath());
  string tileset_path = base_path + "assets/" + tileset_file;

  this->texture = IMG_LoadTexture(this->renderer, tileset_path.c_str());
  if (this->texture == nullptr){
    std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    throw renderer_error();
  }

}

void CharacterLayer::render() {
  this->state->render();
  this->state->animate();
}

