#include "entity.h"

EID EntityManager::generateEid() {
  return lowestUnassignedEid++;
}

Entity* EntityManager::createEntity() {
  EID eid = generateEid();

  return new Entity(eid);
}

void EntityManager::removeEntity(EID eid) {
  for (auto it = this->components.begin(); it != this->components.end(); ++it) {
    auto cid = it->first;

    if (this->components[cid].count(eid)) {
      auto entity = this->components[cid][eid];
      this->components[cid].erase(eid);
      delete entity;
    }
  }
}

