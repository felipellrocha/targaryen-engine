#ifndef ITEMSLAYER_H
#define ITEMSLAYER_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <vector>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/node.h"
#include "layers/itemlayer.h"

#include "physics/AABBTree.h"

using json = nlohmann::json;
using namespace std;

class ItemsLayer : public Node {

  protected:
    vector<shared_ptr<Node>> nodes;

  public:
    ItemsLayer(SDL_Renderer *renderer, vector<shared_ptr<IAABB>> *world, json data, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;
};

#endif

