#ifndef SIXTILE_H
#define SIXTILE_H

#include <SDL2/SDL.h>
#include "renderer/surrounding.h"
#include "renderer/offset.h"

class SixTile : public Tile {
  public:
    SixTile(int setIndex, int tileIndex, int locationIndex)
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
      if (TileSurrounding::SOUTH & this->surrounding
        && TileSurrounding::WEST & this->surrounding
        && TileSurrounding::SOUTHWEST & this->surrounding)
        return offset(1, 4);
      if (TileSurrounding::SOUTH & this->surrounding
        && TileSurrounding::WEST & this->surrounding)
        return offset(2, 1);

      if (TileSurrounding::SOUTH & this->surrounding)
        return offset(0, 4);
      if (TileSurrounding::WEST & this->surrounding)
        return offset(1, 5);

      return offset(0, 5);
    }

    offset southEastOffset() {
      if (TileSurrounding::SOUTH & this->surrounding
        && TileSurrounding::EAST & this->surrounding
        && TileSurrounding::SOUTHEAST & this->surrounding)
        return offset(2, 4);
      if (TileSurrounding::SOUTH & this->surrounding
        && TileSurrounding::EAST & this->surrounding)
        return offset(3, 1);

      if (TileSurrounding::SOUTH & this->surrounding)
        return offset(3, 4);
      if (TileSurrounding::EAST & this->surrounding)
        return offset(2, 5);

      return offset(3, 5);
    }

    offset northEastOffset() {
      if (TileSurrounding::NORTH & this->surrounding
        && TileSurrounding::EAST & this->surrounding
        && TileSurrounding::NORTHEAST & this->surrounding)
        return offset(2, 3);
      if (TileSurrounding::NORTH & this->surrounding
        && TileSurrounding::EAST & this->surrounding)
        return offset(3, 0);

      if (TileSurrounding::NORTH & this->surrounding)
        return offset(3, 3);
      if (TileSurrounding::EAST & this->surrounding)
        return offset(2, 2);

      return offset(3, 2);
    }

    offset northWestOffset() {
      if (TileSurrounding::NORTH & this->surrounding
        && TileSurrounding::WEST & this->surrounding
        && TileSurrounding::NORTHWEST & this->surrounding)
        return offset(1, 3);
      if (TileSurrounding::NORTH & this->surrounding
        && TileSurrounding::WEST & this->surrounding)
        return offset(2, 0);

      if (TileSurrounding::NORTH & this->surrounding)
        return offset(0, 3);
      if (TileSurrounding::WEST & this->surrounding)
        return offset(1, 2);

      return offset(0, 2);
    }
};

#endif
