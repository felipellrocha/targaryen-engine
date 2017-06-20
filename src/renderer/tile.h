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

    void render(Tileset *tileset, Grid *grid, GPU_Target *renderer, int w, int h, int x, int y) {
      GPU_Rect src = {
        (float)(tileset->getX(this->tileIndex)) * w,
        (float)(tileset->getY(this->tileIndex)) * h,
        (float)w,
        (float)h
      };

      GPU_Blit(
        tileset->texture,
        &src,
        renderer,
        (grid->getX(locationIndex)) * w + x,
        (grid->getY(locationIndex)) * h + y
      );
    }
};

#endif
