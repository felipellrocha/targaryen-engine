#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include "renderer/compass.h"

class Tile {
  public:
    int setIndex;
    int tileIndex;
    int locationIndex;
    int surrounding;

    Tile(int setIndex, int tileIndex, int locationIndex)
      : setIndex(setIndex), tileIndex(tileIndex), locationIndex(locationIndex) {}

    void render(Tileset *tileset, Grid *grid, SDL_Renderer *renderer, int w, int h) {
      SDL_Rect src = {
        (tileset->getX(this->tileIndex)) * w,
        (tileset->getY(this->tileIndex)) * h,
        w,
        h
      };

      SDL_Rect dst = {
        (grid->getX(locationIndex)) * w,
        (grid->getY(locationIndex)) * h,
        w,
        h
      };

      SDL_RenderCopy(renderer, tileset->texture, &src, &dst);
    }
};

#endif
