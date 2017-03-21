#ifndef ITEMSLAYER_H
#define ITEMSLAYER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "json/json.hpp"
#include "exceptions.h"
#include "renderer/node.h"
#include "layers/itemlayer.h"

using json = nlohmann::json;
using namespace std;

class ItemsLayer : public Node {

  protected:
    vector<Node *> nodes;


  public:
    ItemsLayer(SDL_Renderer *renderer, json data, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;
};

#endif

