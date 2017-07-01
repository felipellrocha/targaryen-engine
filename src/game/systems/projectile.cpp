#include "projectile.h"

void ProjectileSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<ProjectileComponent>(); 
  EID player = manager->getSpecial("player");
  auto walk = manager->getComponent<WalkComponent>(player);
  auto playerRender = manager->getComponent<RenderComponent>(player);
  auto playerPosition = manager->getComponent<PositionComponent>(player);

  for (EID entity : entities) {
    auto projectile = manager->getComponent<ProjectileComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto collision = manager->getComponent<CollisionComponent>(entity);

    if (collision->isColliding) {
      manager->removeEntity(entity);
      game->addTransition<FadeOutTransition>(1.5f);

      for (auto it : collision->collisions) {
        EID coll = it.first;
        auto health = manager->getComponent<HealthComponent>(coll);

        if (health) {
          health->hearts -= 1;
          if (health->hearts < 0) manager->removeEntity(coll);
        }
      }
      continue;
    }

    if (Compass::NORTH & position->direction) position->nextY -= projectile->vec; 
    if (Compass::EAST & position->direction) position->nextX += projectile->vec; 
    if (Compass::SOUTH & position->direction) position->nextY += projectile->vec; 
    if (Compass::WEST & position->direction) position->nextX -= projectile->vec;
  }

  if (Actions::MAIN & this->game->actions) {
    if (timing > 1.75f) {
      int direction = 0;
      int xOffset = 0;
      int yOffset = 0;

      if (Compass::NORTH & walk->direction) {
        direction = Compass::NORTH;
        yOffset = -48;
      }
      if (Compass::EAST & walk->direction) {
        direction = Compass::EAST;
        xOffset = 48;
      }
      if (Compass::SOUTH & walk->direction) {
        direction = Compass::SOUTH;
        yOffset = 48;
      }
      if (Compass::WEST & walk->direction) {
        direction = Compass::WEST;
        xOffset = -48;
      }

      Entity* attack = this->manager->createEntity();

      manager->addComponent<RenderComponent>(attack, playerRender->layer);
      manager->addComponent<ProjectileComponent>(attack, 8);
      manager->addComponent<DimensionComponent>(attack, 48, 48);
      manager->addComponent<CollisionComponent>(
        attack,
        false,
        Resolver::EXPLOSION,
        12, 12, 24, 24
      );
      manager->addComponent<PositionComponent>(
        attack,
        playerPosition->x + xOffset,
        playerPosition->y + yOffset,
        direction
      );
      manager->addComponent<SpriteComponent>(
        attack,
        0, 0, 48, 48,
        this->game->textures["flame"]
      );

      timing = 0.f;
    }
    timing += dt;
  }
};

