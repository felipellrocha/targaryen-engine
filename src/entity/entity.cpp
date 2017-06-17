#include "entity.h"

EID EntityManager::getSpecial(string key) {
  return this->special[key];
}

void EntityManager::saveSpecial(string key, EID entity) {
  this->special[key] = entity;
}

void EntityManager::saveSpecial(string key, Entity *entity) {
  this->special[key] = entity->eid;
}

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
