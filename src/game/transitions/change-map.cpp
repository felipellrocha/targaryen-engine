#include "change-map.h"

void FadeOutTransition::begin(EntityManager* manager, Renderer* game) {
  Entity* entity = manager->createEntity();
  manager->addComponent<PositionComponent>(entity, 0, 0);
  manager->addComponent<DimensionComponent>(entity, game->windowWidth, game->windowHeight);
  manager->addComponent<ColorComponent>(entity, 0, 0, 0, 0);
  manager->addComponent<RenderComponent>(entity, 999);
  manager->saveSpecial("fade", entity);

  EID player = manager->getSpecial("player");
  manager->removeComponent<InputComponent>(player);
  auto render = manager->getComponent<RenderComponent>(player);
  render->layer = 1000;
};

bool FadeOutTransition::tick(EntityManager* manager, Renderer* game, float dt) {
  if (running > duration) {
    return true;
  }
  else {
    EID fade = manager->getSpecial("fade");
    auto color = manager->getComponent<ColorComponent>(fade);
    color->a = (int)((running / duration) * 255);

    running += dt;
    return false;
  }
};

Transition* FadeOutTransition::end(EntityManager* manager, Renderer* game) {
  EID fade = manager->getSpecial("fade");
  manager->removeEntity(fade);

  EID player = manager->getSpecial("player");
  manager->addComponent<InputComponent>(player);

  return NULL;
};
