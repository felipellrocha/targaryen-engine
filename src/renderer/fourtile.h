#ifndef FOURTILE_H
#define FOURTILE_H

#include <SDL2/SDL.h>
#include "renderer/compass.h"
#include "renderer/offset.h"

class FourTile : public Tile {
  public:
    FourTile(int setIndex, int tileIndex, int locationIndex)
      : Tile(setIndex, tileIndex, locationIndex) {}

    void render(Tileset *tileset, Grid *grid, SDL_Renderer *renderer, int w, int h) {
      offset northwestOffset = this->northWestOffset();
      offset northeastOffset = this->northEastOffset();
      offset southeastOffset = this->southEastOffset();
      offset southwestOffset = this->southWestOffset();
      this->renderPart(tileset, grid, renderer, w, h, offset(0, 0), northwestOffset);
      this->renderPart(tileset, grid, renderer, w, h, offset(1, 0), northeastOffset);
      this->renderPart(tileset, grid, renderer, w, h, offset(1, 1), southeastOffset);
      this->renderPart(tileset, grid, renderer, w, h, offset(0, 1), southwestOffset);
    }

    void renderPart(
      Tileset *tileset,
      Grid *grid,
      SDL_Renderer *renderer,
      int w,
      int h,
      offset mainOffset,
      offset tileOffset
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
        (grid->getX(locationIndex) * w) + (width * mainOffset.x),
        (grid->getY(locationIndex) * h) + (height * mainOffset.y),
        width,
        height
      };

      SDL_RenderCopy(renderer, tileset->texture, &src, &dst);
    }

    offset southWestOffset() {
      if (Compass::SOUTH & this->surrounding
        && Compass::WEST & this->surrounding)
        return offset(1, 2);

      if (Compass::SOUTH & this->surrounding)
        return offset(0, 2);
      if (Compass::WEST & this->surrounding)
        return offset(1, 3);

      return offset(0, 3);
    }

    offset southEastOffset() {
      if (Compass::SOUTH & this->surrounding
        && Compass::EAST & this->surrounding)
        return offset(2, 2);

      if (Compass::SOUTH & this->surrounding)
        return offset(3, 2);
      if (Compass::EAST & this->surrounding)
        return offset(2, 3);

      return offset(3, 3);
    }

    offset northEastOffset() {
      if (Compass::NORTH & this->surrounding
        && Compass::EAST & this->surrounding)
        return offset(2, 1);

      if (Compass::NORTH & this->surrounding)
        return offset(3, 1);
      if (Compass::EAST & this->surrounding)
        return offset(2, 0);

      return offset(3, 0);
    }

    offset northWestOffset() {
      if (Compass::NORTH & this->surrounding
        && Compass::WEST & this->surrounding)
        return offset(1, 1);

      if (Compass::NORTH & this->surrounding)
        return offset(0, 1);
      if (Compass::WEST & this->surrounding)
        return offset(1, 0);

      return offset(0, 0);
    }
};

#endif
