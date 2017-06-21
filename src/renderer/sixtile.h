#ifndef SIXTILE_H
#define SIXTILE_H

#include <vector>
#include <array>
#include "sdl2image.h"
#include "renderer/compass.h"
#include "renderer/offset.h"

using namespace std;

struct sixTile {
  static offset southWestOffset(int surrounding) {
    if (Compass::SOUTH & surrounding
      && Compass::WEST & surrounding
      && Compass::SOUTHWEST & surrounding)
      return offset(1, 4);
    if (Compass::SOUTH & surrounding
      && Compass::WEST & surrounding)
      return offset(2, 1);

    if (Compass::SOUTH & surrounding)
      return offset(0, 4);
    if (Compass::WEST & surrounding)
      return offset(1, 5);

    return offset(0, 5);
  }

  static offset southEastOffset(int surrounding) {
    if (Compass::SOUTH & surrounding
      && Compass::EAST & surrounding
      && Compass::SOUTHEAST & surrounding)
      return offset(2, 4);
    if (Compass::SOUTH & surrounding
      && Compass::EAST & surrounding)
      return offset(3, 1);

    if (Compass::SOUTH & surrounding)
      return offset(3, 4);
    if (Compass::EAST & surrounding)
      return offset(2, 5);

    return offset(3, 5);
  }

  static offset northEastOffset(int surrounding) {
    if (Compass::NORTH & surrounding
      && Compass::EAST & surrounding
      && Compass::NORTHEAST & surrounding)
      return offset(2, 3);
    if (Compass::NORTH & surrounding
      && Compass::EAST & surrounding)
      return offset(3, 0);

    if (Compass::NORTH & surrounding)
      return offset(3, 3);
    if (Compass::EAST & surrounding)
      return offset(2, 2);

    return offset(3, 2);
  }

  static offset northWestOffset(int surrounding) {
    if (Compass::NORTH & surrounding
      && Compass::WEST & surrounding
      && Compass::NORTHWEST & surrounding)
      return offset(1, 3);
    if (Compass::NORTH & surrounding
      && Compass::WEST & surrounding)
      return offset(2, 0);

    if (Compass::NORTH & surrounding)
      return offset(0, 3);
    if (Compass::WEST & surrounding)
      return offset(1, 2);

    return offset(0, 2);
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
      sixTile::northWestOffset(surrounding),
      sixTile::northEastOffset(surrounding),
      sixTile::southWestOffset(surrounding),
      sixTile::southEastOffset(surrounding)
    };
  }
};

#endif
