#ifndef ITEMLAYER_H
#define ITEMLAYER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "json/json.hpp"
#include "exceptions.h"
#include "renderer/node.h"

using json = nlohmann::json;
using namespace std;

class ItemLayer : public Node {

  protected:
    vector<int> layers;

    int tile_x, tile_y,
        location_x, location_y,
        w, h;

  public:
    ItemLayer(SDL_Renderer *renderer, json data, int index, int tileType, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;
};

#endif


