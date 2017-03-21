#include "itemslayer.h"

ItemsLayer::ItemsLayer(SDL_Renderer *renderer, json data, int layer) {

  this->renderer = renderer;

  auto tiles = data.at("layers").at(layer).at("data").get<vector<int>>();
  for (uint i = 0; i < tiles.size(); i++) {
    Node * n = nullptr;

    if (tiles[i] > 0) {
      n = new ItemLayer(renderer, data, i, tiles[i], layer);
      this->nodes.push_back(n);
    }
  }
}

void ItemsLayer::render() {
  for (uint i = 0; i < this->nodes.size(); i++) this->nodes[i]->render();
}

void ItemsLayer::input(SDL_Event event) {
}

void ItemsLayer::update() {
}

