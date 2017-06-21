#ifndef SIMPLETILE_H
#define SIMPLETILE_H

#include <vector>
#include <array>
#include "sdl2image.h"
#include "renderer/compass.h"
#include "renderer/offset.h"

using namespace std;

struct simpleTile {
  static vector<array<SDL_Rect, 2>> calculateAll(TileComponent *tile, Tileset *tileset, Grid grid) {
    int width = grid.tile_w;
    int height = grid.tile_h;

    SDL_Rect src = SDL_Rect{
      (tileset->getX(tile->tileIndex) * width),
      (tileset->getY(tile->tileIndex) * height),
      width,
      height
    };
    SDL_Rect dst = SDL_Rect{
      (grid.getX(tile->locationIndex) * width),
      (grid.getY(tile->locationIndex) * height),
      width,
      height
    };

    return {{src, dst}};
  }
};

#endif
