#include "staticcollisionlayer.h"

StaticCollisionLayer::StaticCollisionLayer(
  SDL_Renderer *renderer,
  json data
  //int index,
  //int layer
) {

  this->renderer = renderer;

  this->x = data
    .at("x")
    .get<int>();

  this->y = data
    .at("y")
    .get<int>();

  this->w = data
    .at("width")
    .get<int>();

  this->h = data
    .at("height")
    .get<int>();

  // uint is necessary here simply to disambiguate
  // the constructor call
  this->aabb = AABB(
    (uint)this->x,
    (uint)this->y,
    0,
    this->x + this->w,
    this->y + this->h,
    0
  );
}

void StaticCollisionLayer::render() {
}

void StaticCollisionLayer::input(SDL_Event event) {
}

void StaticCollisionLayer::update() {
}


