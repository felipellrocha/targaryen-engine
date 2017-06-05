#include "projectile.h"

void ProjectileSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<ProjectileComponent>(); 
  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto projectile = manager->getComponent<ProjectileComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);

    if (Compass::NORTH & position->direction) position->nextY -= projectile->vec; 
    if (Compass::EAST & position->direction) position->nextX += projectile->vec; 
    if (Compass::SOUTH & position->direction) position->nextY += projectile->vec; 
    if (Compass::WEST & position->direction) position->nextX -= projectile->vec;
  }

  if (Actions::MAIN & this->game->actions) {
    if (timing > 1.5f) {
      Entity* attack = this->manager->createEntity();
      manager->addComponent<CollisionComponent>(attack, new CollisionComponent());
      manager->addComponent<ProjectileComponent>(attack, new ProjectileComponent(4));
      manager->addComponent<DimensionComponent>(attack, new DimensionComponent(48, 48));
      manager->addComponent<PositionComponent>(attack, new PositionComponent(528, 1248+48, Compass::EAST));
      manager->addComponent<SpriteComponent>(attack, new SpriteComponent(0, 0, 48, 48, this->game->textures["flame"]));
      manager->addComponent<RenderComponent>(attack, new RenderComponent());

      timing = 0.f;
    }
    timing += dt;
  }
};

