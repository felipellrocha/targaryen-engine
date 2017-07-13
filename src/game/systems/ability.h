#ifndef SYSTEMABILITY_H
#define SYSTEMABILITY_H

#include "sdl2image.h"
#include "game/components.h"
#include "game/utils.h"

#include "renderer/compass.h"
#include "renderer/renderer.h"

#include "entity/entity.h"
#include "entity/system.h"
#include "game/transitions/change-map.h"

class AbilitySystem : public System {
  public:
    void update(float dt);

    AbilitySystem(EntityManager *_manager, Renderer *_game) :
      System(_manager, _game) { };
};

#endif
