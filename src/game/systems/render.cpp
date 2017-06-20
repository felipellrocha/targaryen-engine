#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  auto cameraPosition = manager->getComponent<PositionComponent>(manager->getSpecial("camera"));
  auto cameraDimension = manager->getComponent<DimensionComponent>(manager->getSpecial("camera"));

  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];

    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto grid = manager->getComponent<GridComponent>(entity);

    if (sprite) {
      GPU_Rect src = {
        (float)sprite->x,
        (float)sprite->y,
        (float)sprite->w,
        (float)sprite->h
      };

			GPU_Blit(
        sprite->texture,
        &src,
        game->ren,
        position->x - cameraPosition->x,
        position->y - cameraPosition->y
      );

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
