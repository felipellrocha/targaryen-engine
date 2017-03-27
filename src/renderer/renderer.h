#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>
#include <memory>
#include <utility>
#include "renderer/node.h"
#include "exceptions.h"
#include "json/json.h"
#include "json/readjson.h"

#include "node.h"
#include "layers/imagelayer.h"
#include "layers/tilelayer.h"
#include "layers/characterlayer.h"
#include "layers/itemslayer.h"

#include "physics/AABBTree.h"
#include "physics/IAABB.h"

using json = nlohmann::json;
using namespace std;

class Renderer {

  protected:
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<IAABB>> world;
    AABBTree tree = AABBTree(100);

  public:
    bool running;

    Renderer(string levelFile);
    ~Renderer();

    void render();
    void physics();
    void input();
    void update();

    bool isRunning() { return running; };
    void quit() { running = false; };

};

#endif
