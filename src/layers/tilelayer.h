#ifndef TILELAYER_H
#define TILELAYER_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <vector>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/node.h"

using json = nlohmann::json;
using namespace std;

class TileLayer : public Node {

  protected:
    int tile_w;
    int tile_h;

    int tile_count_w;
    int tile_count_h;

    int layer_count_w;
    int layer_count_h;

    vector<int> tiles;


  public:
    TileLayer(SDL_Renderer *renderer, json data, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;
};

#endif
