#include "ai.h"

void AISystem::update(float dt) {
  //vector<EID> entities = manager->getAllEntitiesWithComponent<AIComponent>();
  tree->update();
};

