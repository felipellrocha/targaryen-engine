#ifndef TILELAYER_H
#define TILELAYER_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <vector>
#include <array>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/tile.h"
#include "renderer/sixtile.h"
#include "renderer/fourtile.h"
#include "renderer/compass.h"

using json = nlohmann::json;
using namespace std;

class TileLayer {

  protected:
    SDL_Renderer *renderer;
    vector<Tileset *> tilesets;

    int tile_w;
    int tile_h;

    Grid *grid;

    vector<Tile *> tiles;

  public:
    TileLayer(SDL_Renderer *renderer, vector<Tileset *> tilesets, json data, int layer);
    void render(int x, int y);

    void renderSimpleTile(int index, Tileset *tileset, Tile *tile);
    int findSurroundings(Tile *t1, Grid *grid);
};

#endif
