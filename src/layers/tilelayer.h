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
#include "utils.h"

using json = nlohmann::json;
using namespace std;

class TileLayer {

  public:
    SDL_Renderer *renderer;
    vector<Tileset *> tilesets;

    int tile_w;
    int tile_h;

    Grid *grid;

    vector<Tile *> tiles;

    TileLayer(SDL_Renderer *renderer, vector<Tileset *> tilesets, json game_data, json map_data, int layer);
    void render(int x, int y, int w, int h);

    void renderSimpleTile(int index, Tileset *tileset, Tile *tile);
    int findSurroundings(Tile *t1, Grid *grid);
};

#endif
