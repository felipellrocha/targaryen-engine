#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "sdl2image.h"
#include "renderer/renderer.h"
#include "game/components.h"
#include "game/utils.h"

#include "entity/entity.h"
#include "entity/system.h"

#include "renderer/simpletile.h"
#include "renderer/fourtile.h"
#include "renderer/sixtile.h"

class RenderSystem : public System {
  public:
    SDL_Renderer *renderer = nullptr;

    void update(float dt);

    RenderSystem(EntityManager *_manager, Renderer *_game) :
      System(_manager, _game) { };
};

#endif
