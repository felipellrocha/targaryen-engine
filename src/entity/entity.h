#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <map>

#include "entity/component.h"

using namespace std;

typedef uint32_t EID;

struct Entity {
  EID eid;

  Entity(EID _eid) : eid(_eid) {}
};

const int MAX_ENTITY = 8192;
const int NUM_COMPONENTS = 11;

class EntityManager {
  public:
    int lowestUnassignedEid = 0;
    map<CID, map<EID, Component *>> components;

    EID camera;
    EID player;

    EID getCamera() {
      return this->camera;
    }

    EID getPlayer() {
      return this->player;
    }

    void savePlayer(Entity *player) {
      this->player = player->eid;
    }

    void saveCamera(Entity *camera) {
      this->camera = camera->eid;
    }

    EID generateEid();
    Entity* createEntity();
    void removeEntity(EID eid);

    template<class ComponentClass, typename... Args>
    void addComponent(Entity *entity, Args... args) {
      this->components[ComponentClass::cid][entity->eid] = new ComponentClass(args...);
    }

    template<class ComponentClass>
    ComponentClass* getComponent(EID eid)
    {
      return static_cast<ComponentClass *>(this->components[ComponentClass::cid][eid]);
    }

    template<class ComponentClass>
    vector<EID> getAllEntitiesWithComponent()
    {
      vector<EID> entities;
      auto components = this->components[ComponentClass::cid];

      for (auto it = components.begin(); it != components.end(); ++it) {
        entities.push_back(it->first);
      }

      return entities;
    }
};

#endif
