#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  auto cameraPosition = manager->getComponent<PositionComponent>(manager->getCamera());
  auto cameraDimension = manager->getComponent<DimensionComponent>(manager->getCamera());

  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];

    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto grid = manager->getComponent<GridComponent>(entity);

    if (sprite) {
      SDL_Rect src = {
        sprite->x,
        sprite->y,
        sprite->w,
        sprite->h
      };

      SDL_Rect dst = {
        position->x - cameraPosition->x,
        position->y - cameraPosition->y,
        sprite->w,
        sprite->h
      };
			SDL_RenderCopy(game->ren, sprite->texture, &src, &dst);

    }
    else {
      grid->layer.render(
        cameraPosition->x,
        cameraPosition->y,
        cameraDimension->w,
        cameraDimension->h
      );
    }
  }
};
