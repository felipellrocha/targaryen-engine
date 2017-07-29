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

//#include "AI/node.h"
//#include "AI/behaviortree.h"
#include "AI/composite.h"
#include "AI/decorator.h"

#include "renderer/tileset.h"
#include "renderer/geometry.h"
#include "renderer/animation.h"

#include "game/components.h"
#include "game/behaviors/follow.h"
#include "game/behaviors/proximity.h"
#include "game/utils.h"
#include "game/systems/ability.h"
#include "game/systems/render.h"
#include "game/systems/input.h"
#include "game/systems/camera.h"
#include "game/systems/collision.h"
#include "game/systems/projectile.h"
#include "game/systems/walk.h"
#include "game/systems/transition.h"
#include "game/systems/ai.h"
#include "game/materials.h"

using json = nlohmann::json;
using namespace std;

class Node;
class Transition;
class Renderer {
public:
  vector<Tileset *> tilesets;
  vector<System *> systems;

  SDL_Window *win = nullptr;
  SDL_Renderer *ren = nullptr;
  SDL_GLContext context = nullptr;

  int windowWidth = 1100;
  int windowHeight = 600;

  bool running = true;
  int compass = 0;
  int actions = 0;

  int numTransitions = 0;

  map<string, string> mapsByName;
  map<EID, Node*> behaviors;

  string gamePackage;
  EntityManager* manager;

  json entities;
  map<string, Animation> animations;
  forward_list<Transition *> incoming;
  forward_list<Transition *> outgoing;
  set<Transition *> transitions;

  Grid grid;
  map<string, SDL_Texture*> textures;

  void loop(float dt);
  bool isRunning() { return running; };
  void quit() { running = false; };

  template<class BehaviorClass, typename... Args>
  BehaviorClass* makeBehavior(Args... args) {
    return new BehaviorClass(this, manager, args...);
  }

  template<class SystemClass, typename... Args>
  void registerSystem(Args... args) {
    SystemClass *system = new SystemClass(args..., this);
    this->systems.push_back(system);
  }

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
