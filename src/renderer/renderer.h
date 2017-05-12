#ifndef RENDERER_H
#define RENDERER_H

#ifdef DRAW_FPS
#include <SDL2_ttf/SDL_ttf.h>
#endif

#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <memory>
#include <utility>
#include "exceptions.h"
#include "json/json.h"
#include "json/readjson.h"

#include "entity/entity.h"
#include "entity/system.h"

#include "game/components.h"
#include "game/systems/render.h"
#include "game/systems/input.h"
#include "game/systems/camera.h"
#include "game/systems/collision.h"

using json = nlohmann::json;
using namespace std;

class Renderer {
  public:
    vector<Tileset *> tilesets;
    vector<System *> systems;

    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    bool running = true;
    int compass = 0;
    Grid grid;

    int windowWidth = 1280;
    int windowHeight = 680;

    bool isRunning() { return running; };
    void quit() { running = false; };

    void registerSystem(System *system);
    void loop(float dt);

    Renderer(string levelFile);
    ~Renderer();
};

#endif
