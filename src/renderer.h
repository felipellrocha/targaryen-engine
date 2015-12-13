#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include "json.hpp"
#include "node.h"
#include "exceptions.h"
#include "readjson.h"

#include "node.h"
#include "imagelayer.h"
#include "tilelayer.h"

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
};

#endif
