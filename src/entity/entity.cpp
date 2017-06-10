#include "entity.h"

EID EntityManager::generateEid() {
  return lowestUnassignedEid++;
}

Entity* EntityManager::createEntity() {
  EID eid = generateEid();
  this->entities.push_back(eid);

  return new Entity(eid);
}
