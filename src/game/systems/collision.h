#ifndef SYSTEMCOLLISION_H
#define SYSTEMCOLLISION_H

#include <SDL2/SDL.h>
#include <cstdlib>

#include "game/components.h"
#include "renderer/renderer.h"
#include "renderer/compass.h"
#include "entity/entity.h"
#include "entity/system.h"
#include "utils.h"

using namespace std;

class CollisionSystem : public System {
  public:
    void update(float dt);

    CollisionSystem(EntityManager *_manager, Renderer *_game) :
      System(_manager, _game) { };
};

#endif
