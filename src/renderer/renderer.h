#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <utility>
#include "sdl2image.h"
#include "exceptions.h"
#include "json/json.h"
#include "json/readjson.h"

#include "entity/entity.h"
#include "entity/system.h"

#include "renderer/tileset.h"
#include "game/components.h"
#include "game/systems/render.h"
#include "game/systems/input.h"
#include "game/systems/camera.h"
#include "game/systems/collision.h"
#include "game/systems/projectile.h"
#include "game/systems/walk.h"

using json = nlohmann::json;
using namespace std;

struct Animation {
    string id;
    int numberOfFrames;
    map<int, SDL_Rect> keyframes;
};

class Renderer {
  public:
    vector<Tileset *> tilesets;
    vector<System *> systems;

    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    int windowWidth = 1100;
    int windowHeight = 600;

    bool running = true;
    int compass = 0;
    int actions = 0;

    json entities;
    map<string, Animation> animations;

    Grid grid;
    map<string, SDL_Texture*> textures;

    bool isRunning() { return running; };
    void quit() { running = false; };

    void registerSystem(System *system);
    void loop(float dt);

    Renderer(string levelFile);
    ~Renderer();
};

#endif
