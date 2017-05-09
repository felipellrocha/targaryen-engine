#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "entity/component.h"

using namespace std;

typedef uint32_t EID;

struct Entity {
  EID eid;

  Entity(EID _eid) : eid(_eid) {}
};

class EntityManager {
  public:
    int lowestUnassignedEid = 0;
    vector<EID> entities;
    map<EID, map<CID, Component *>> components;
    EID generateEid();

    Entity* createEntity();

    template<class ComponentClass> void addComponent(Entity *entity, Component *component) {
      this->components[entity->eid][ComponentClass::cid] = component;
    }

    template<class ComponentClass> ComponentClass* getComponent(EID eid)
    {
      return static_cast<ComponentClass *>(this->components[eid][ComponentClass::cid]);
    }

    template<class ComponentClass>
    vector<EID> getAllEntitiesWithComponent()
    {
      vector<EID> entityList;
      for (int i = 0; i < this->entities.size(); i++) {
        EID eid = this->entities[i];
        if (this->components.count(eid))
        if (this->components[eid].count(ComponentClass::cid)) {
          entityList.push_back(eid);
        }
      }
      return entityList;
    }
};

#endif
