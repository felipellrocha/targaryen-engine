#ifndef SIXTILE_H
#define SIXTILE_H

#include <SDL2/SDL.h>
#include "renderer/compass.h"
#include "renderer/offset.h"

class SixTile : public Tile {
  public:
    SixTile(int setIndex, int tileIndex, int locationIndex)
      : Tile(setIndex, tileIndex, locationIndex) {}

    void render(Tileset *tileset, Grid *grid, SDL_Renderer *renderer, int w, int h, int x, int y) {
      offset northwestOffset = this->northWestOffset();
      offset northeastOffset = this->northEastOffset();
      offset southeastOffset = this->southEastOffset();
      offset southwestOffset = this->southWestOffset();
      this->renderPart(tileset, grid, renderer, w, h, offset(0, 0), northwestOffset, x, y);
      this->renderPart(tileset, grid, renderer, w, h, offset(1, 0), northeastOffset, x, y);
      this->renderPart(tileset, grid, renderer, w, h, offset(1, 1), southeastOffset, x, y);
      this->renderPart(tileset, grid, renderer, w, h, offset(0, 1), southwestOffset, x, y);
    }

    void renderPart(
      Tileset *tileset,
      Grid *grid,
      SDL_Renderer *renderer,
      int w,
      int h,
      offset mainOffset,
      offset tileOffset,
      int x,
      int y
    ) {

      int width = w / 2;
      int height = h / 2;
      
      SDL_Rect src = {
        (tileset->getX(this->tileIndex) * w) + (width * tileOffset.x),
        (tileset->getY(this->tileIndex) * h) + (height * tileOffset.y),
        width,
        height
      };

      SDL_Rect dst = {
        (grid->getX(locationIndex) * w) + (width * mainOffset.x) + x,
        (grid->getY(locationIndex) * h) + (height * mainOffset.y) + y,
        width,
        height
      };

      SDL_RenderCopy(renderer, tileset->texture, &src, &dst);
    }

    offset southWestOffset() {
      if (Compass::SOUTH & this->surrounding
        && Compass::WEST & this->surrounding
        && Compass::SOUTHWEST & this->surrounding)
        return offset(1, 4);
      if (Compass::SOUTH & this->surrounding
        && Compass::WEST & this->surrounding)
        return offset(2, 1);

      if (Compass::SOUTH & this->surrounding)
        return offset(0, 4);
      if (Compass::WEST & this->surrounding)
        return offset(1, 5);

      return offset(0, 5);
    }

    offset southEastOffset() {
      if (Compass::SOUTH & this->surrounding
        && Compass::EAST & this->surrounding
        && Compass::SOUTHEAST & this->surrounding)
        return offset(2, 4);
      if (Compass::SOUTH & this->surrounding
        && Compass::EAST & this->surrounding)
        return offset(3, 1);

      if (Compass::SOUTH & this->surrounding)
        return offset(3, 4);
      if (Compass::EAST & this->surrounding)
        return offset(2, 5);

      return offset(3, 5);
    }

    offset northEastOffset() {
      if (Compass::NORTH & this->surrounding
        && Compass::EAST & this->surrounding
        && Compass::NORTHEAST & this->surrounding)
        return offset(2, 3);
      if (Compass::NORTH & this->surrounding
        && Compass::EAST & this->surrounding)
        return offset(3, 0);

      if (Compass::NORTH & this->surrounding)
        return offset(3, 3);
      if (Compass::EAST & this->surrounding)
        return offset(2, 2);

      return offset(3, 2);
    }

    offset northWestOffset() {
      if (Compass::NORTH & this->surrounding
        && Compass::WEST & this->surrounding
        && Compass::NORTHWEST & this->surrounding)
        return offset(1, 3);
      if (Compass::NORTH & this->surrounding
        && Compass::WEST & this->surrounding)
        return offset(2, 0);

      if (Compass::NORTH & this->surrounding)
        return offset(0, 3);
      if (Compass::WEST & this->surrounding)
        return offset(1, 2);

      return offset(0, 2);
    }
};

#endif
