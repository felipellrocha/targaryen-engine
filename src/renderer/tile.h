#ifndef TILE_H
#define TILE_H

#include "sdl2image.h"
#include <string>
#include "renderer/compass.h"
#include "renderer/tileset.h"

class Tile {
  public:
    int setIndex;
    int tileIndex;
    int locationIndex;
    int surrounding;

    Tile(int setIndex, int tileIndex, int locationIndex)
      : setIndex(setIndex), tileIndex(tileIndex), locationIndex(locationIndex) {}

    void render(Tileset *tileset, Grid *grid, SDL_Renderer *renderer, int w, int h, int x, int y) {
      SDL_Rect src = {
        (tileset->getX(this->tileIndex)) * w,
        (tileset->getY(this->tileIndex)) * h,
        w,
        h
      };

      SDL_Rect dst = {
        (grid->getX(locationIndex)) * w + x,
        (grid->getY(locationIndex)) * h + y,
        w,
        h
      };

      SDL_RenderCopy(renderer, tileset->texture, &src, &dst);
    }
};

#endif
