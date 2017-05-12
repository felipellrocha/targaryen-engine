#include "camera.h"

void CameraSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<CenteredCameraComponent>(); 
  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto camera = manager->getComponent<CenteredCameraComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto dimension = manager->getComponent<DimensionComponent>(entity);

    auto center = manager->getComponent<PositionComponent>(camera->entity);

    position->x = max(center->x - dimension->w / 2, 0);
    position->y = max(center->y - dimension->h / 2, 0);
    position->x = min(position->x, game->grid.columns * game->grid.tile_w - dimension->w);
    position->y = min(position->y, game->grid.rows * game->grid.tile_h - dimension->h);
  }
};

