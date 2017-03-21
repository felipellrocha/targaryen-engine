#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include "renderer/node.h"
#include "exceptions.h"
#include "json/json.hpp"
#include "json/readjson.h"

#include "node.h"
#include "layers/imagelayer.h"
#include "layers/tilelayer.h"
#include "layers/characterlayer.h"
#include "layers/itemslayer.h"

using json = nlohmann::json;
using namespace std;

class Renderer {

  protected:
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    vector<Node *> nodes;

  public:
    Renderer(string levelFile);
    ~Renderer();

    void render();
    void input();
    void update();
};

#endif
