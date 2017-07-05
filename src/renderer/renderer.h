#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <forward_list>
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
#include "renderer/animation.h"
//#include "renderer/transition.h"
#include "game/components.h"
#include "game/utils.h"
#include "game/systems/render.h"
#include "game/systems/input.h"
#include "game/systems/camera.h"
#include "game/systems/collision.h"
#include "game/systems/projectile.h"
#include "game/systems/walk.h"
#include "game/systems/transition.h"

using json = nlohmann::json;
using namespace std;

class Transition;
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

    int numTransitions = 0;

    map<string, string> mapsByName;

    string gamePackage;
    EntityManager* manager;

    json entities;
    map<string, Animation> animations;
    forward_list<Transition *> incoming;
    forward_list<Transition *> outgoing;
    set<Transition *> transitions;

    Grid grid;
    map<string, SDL_Texture*> textures;

    bool isRunning() { return running; };
    void quit() { running = false; };

    template<class SystemClass, typename... Args>
    void registerSystem(Args... args) {
      SystemClass *system = new SystemClass(args..., this);
      this->systems.push_back(system);
    }

    void loop(float dt);

    template<class TransitionClass, typename... Args>
    void addTransition(Args... args) {
      TransitionClass *transition = new TransitionClass(args...);
      incoming.push_front(transition);
    }

    void addTransition(Transition* transition) {
      incoming.push_front(transition);
    }

    void loadStage(string level);
    void loadStage(json game_data, string level);
    void runScript(json commands);

    Renderer(string _gamePackage, EntityManager* _manager);
    ~Renderer();
};

#endif
