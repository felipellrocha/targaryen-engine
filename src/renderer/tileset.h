#ifndef TILESET_H
#define TILESET_H

#include <SDL2/SDL.h>
#include "sdl2image.h"

using namespace std;

class Grid {
  public:
    int rows = 0;
    int columns = 0;
    int tile_w = 0;
    int tile_h = 0;

    Grid(int rows, int columns) : rows(rows), columns(columns) {}
    Grid() : Grid(0, 0) { };

    bool isInside(int x, int y) {
      return x >= 0 && x < this->columns && y >= 0 && y < this->rows;
    }

    int getX(int index) {
      return index % this->columns;
    }

    int getY(int index) {
      return index / this->columns;
    }

    int getIndex(int x, int y) {
      return (y * this->columns) + x;
    }
};

class Tileset : public Grid {
  public:
    string type;
    SDL_Texture *texture = nullptr;
    map<int, string> terrains;

    Tileset(int rows, int columns, string type, SDL_Texture *texture)
      : Grid(rows, columns), type(type), texture(texture) {}

    Tileset(int rows, int columns, string type, SDL_Texture *texture, map<int, string> terrains)
      : Grid(rows, columns), type(type), texture(texture), terrains(terrains) {}
};

#endif
