#include "itemslayer.h"

ItemsLayer::ItemsLayer(SDL_Renderer *renderer, vector<shared_ptr<IAABB>> *world, json data, int layer) {

  this->renderer = renderer;

  auto tiles = data.at("layers").at(layer).at("data").get<vector<int>>();
  for (uint i = 0; i < tiles.size(); i++) {

    if (tiles[i] > 0) {
      shared_ptr<ItemLayer> node = make_shared<ItemLayer>(
        renderer,
        data,
        i,
        tiles[i],
        layer
      );

      this->nodes.push_back(node);
      world->push_back(node);
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

