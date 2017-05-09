#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "sdl2image.h"
#include <string>
#include <memory>
#include <utility>
#include "renderer/node.h"
#include "exceptions.h"
#include "json/json.h"
#include "json/readjson.h"

#include "node.h"
#include "tileset.h"
#include "layers/imagelayer.h"
#include "layers/tilelayer.h"
#include "layers/characterlayer.h"
#include "layers/staticcollisionlayer.h"

#include "physics/AABBTree.h"
#include "physics/IAABB.h"

#include "entity/entity.h"
#include "entity/system.h"

#include "game/components.h"
#include "game/systems/render.h"
#include "game/systems/input.h"

using json = nlohmann::json;
using namespace std;

class Renderer {

  protected:
    vector<Tileset *> tilesets;
    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<IAABB>> world;
    AABBTree tree = AABBTree(1);

  public:
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    bool running;

    Renderer(string levelFile);
    ~Renderer();

    void render();
    void physics();
    void update();

    bool isRunning() { return running; };
    void quit() { running = false; };




    vector<System *> systems;

    void registerSystem(System *system);
    void loop(float dt);


};

#endif
