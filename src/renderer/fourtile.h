#ifndef FOURTILE_H
#define FOURTILE_H

#include <array>
#include "sdl2image.h"
#include "renderer/compass.h"
#include "renderer/offset.h"

using namespace std;

struct fourTile {
  static offset southWestOffset(int surrounding) {
    if (Compass::SOUTH & surrounding
      && Compass::WEST & surrounding)
      return offset(1, 2);

    if (Compass::SOUTH & surrounding)
      return offset(0, 2);
    if (Compass::WEST & surrounding)
      return offset(1, 3);

    return offset(0, 3);
  }

  static offset southEastOffset(int surrounding) {
    if (Compass::SOUTH & surrounding
      && Compass::EAST & surrounding)
      return offset(2, 2);

    if (Compass::SOUTH & surrounding)
      return offset(3, 2);
    if (Compass::EAST & surrounding)
      return offset(2, 3);

    return offset(3, 3);
  }

  static offset northEastOffset(int surrounding) {
    if (Compass::NORTH & surrounding
      && Compass::EAST & surrounding)
      return offset(2, 1);

    if (Compass::NORTH & surrounding)
      return offset(3, 1);
    if (Compass::EAST & surrounding)
      return offset(2, 0);

    return offset(3, 0);
  }

  static offset northWestOffset(int surrounding) {
    if (Compass::NORTH & surrounding
      && Compass::WEST & surrounding)
      return offset(1, 1);

    if (Compass::NORTH & surrounding)
      return offset(0, 1);
    if (Compass::WEST & surrounding)
      return offset(1, 0);

    return offset(0, 0);
  }

  static vector<array<SDL_Rect, 2>> calculateAll(TileComponent *tile, Tileset *tileset, Grid grid) {
    int width = grid.tile_w / 2;
    int height = grid.tile_h / 2;

    auto calcs = calculateOffset(tile->surroundings);
    vector<array<SDL_Rect, 2>> out(4);

    for (int i = 0; i < 4; ++i) {
      offset calc = calcs[i];

      SDL_Rect src = SDL_Rect{
        (tileset->getX(tile->tileIndex) * grid.tile_w) + (calc.x * width),
        (tileset->getY(tile->tileIndex) * grid.tile_h) + (calc.y * height),
        width,
        height
      };
      SDL_Rect dst = SDL_Rect{
        (grid.getX(tile->locationIndex) * grid.tile_w) + ((i % 2) * width),
        (grid.getY(tile->locationIndex) * grid.tile_h) + ((i / 2) * height),
        width,
        height
      };
      out[i] = {{src, dst}};
    }

    return out;
  }

  static array<offset, 4> calculateOffset(int surrounding) {
    return {
      fourTile::northWestOffset(surrounding),
      fourTile::northEastOffset(surrounding),
      fourTile::southWestOffset(surrounding),
      fourTile::southEastOffset(surrounding)
    };
  }
};

#endif
