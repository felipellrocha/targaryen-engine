#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto grid = manager->getComponent<GridComponent>(entity);
    auto camera = manager->getComponent<PositionComponent>(manager->getCamera());

    if (sprite) {
      SDL_Rect src = {
        sprite->x,
        sprite->y,
        sprite->w,
        sprite->h
      };

      SDL_Rect dst = {
        position->x - camera->x,
        position->y - camera->y,
        sprite->w,
        sprite->h
      };
			SDL_RenderCopy(game->ren, sprite->texture, &src, &dst);
    }
    else {
      grid->layer.render(camera->x, camera->y);
    }

  }

};
