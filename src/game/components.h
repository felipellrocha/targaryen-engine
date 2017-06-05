#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <iostream>

#include "entity/entity.h"
#include "entity/component.h"
#include "layers/tilelayer.h"

struct HealthComponent : public Component {
  static CID cid;

  int hearts;
  int max;

  HealthComponent(int _hearts, int _max) : hearts(_hearts), max(_max) {}

};

struct PositionComponent : public Component {
  static CID cid;

  int x;
  int y;

  int nextX;
  int nextY;

  int direction = 0;

  PositionComponent(int _x, int _y)
    : x(_x), y(_y), nextX(_x), nextY(_y) { }
  PositionComponent(int _x, int _y, int _direction)
    : x(_x), y(_y), nextX(_x), nextY(_y), direction(_direction) { }
};

struct DimensionComponent : public Component {
  static CID cid;

  int w;
  int h;

  DimensionComponent(int _w, int _h) : w(_w), h(_h) {}
};

struct SpriteComponent : public Component {
  static CID cid;

  int x;
  int y;
  int w;
  int h;
  string src;
  SDL_Texture *texture;

  SpriteComponent(int _x, int _y, int _w, int _h, SDL_Texture *_texture)
      : x(_x), y(_y), w(_w), h(_h), texture(_texture) {
  }
};

struct InputComponent : public Component {
  static CID cid;

  InputComponent() {};
};

struct MovementComponent : public Component {
  static CID cid;

  int vecX;
  int vecY;

  MovementComponent(int _vecX, int _vecY)
    : vecX(_vecX), vecY(_vecY) { };
};

struct GridComponent : public Component {
  static CID cid;

  TileLayer layer;

  GridComponent(TileLayer _layer)
    : layer(_layer) { };
};

struct RenderComponent : public Component {
  static CID cid;

  RenderComponent() { };
};

struct CenteredCameraComponent : public Component {
  static CID cid;

  EID entity;

  CenteredCameraComponent(EID _entity) : entity(_entity) { };
};

struct CollisionComponent : public Component {
  static CID cid;

  // Static colliders are things like walls, and such, that are never moving
  // keeping track of them allows us to run a small optimization until we need
  // some more heavy duty things to check for collision
  bool isStatic = false;
  bool isColliding = false;

  CollisionComponent(bool _isStatic) : isStatic(_isStatic) { };
  CollisionComponent() : CollisionComponent(false) { };
};

struct ProjectileComponent : public Component {
  static CID cid;

  int vec = 0;

  ProjectileComponent(int _vec)
    : vec(_vec) { };
};

#endif
