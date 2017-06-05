#include "input.h"

void InputSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<MovementComponent>(); 
  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto movement = manager->getComponent<MovementComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);

    position->direction = this->game->compass;

    if (Compass::NORTH & this->game->compass) {
      position->nextY -= movement->vecY; 
    }
    if (Compass::EAST & this->game->compass) {
      position->nextX += movement->vecX; 
    }
    if (Compass::SOUTH & this->game->compass) {
      position->nextY += movement->vecY; 
    }
    if (Compass::WEST & this->game->compass) {
      position->nextX -= movement->vecX;
    }
  }
};

