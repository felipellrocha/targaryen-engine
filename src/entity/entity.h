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
    map<string, EID> special;

    EID getSpecial(string key);
    void saveSpecial(string key, EID entity);
    void saveSpecial(string key, Entity *entity);

    EID generateEid();
    Entity* createEntity();
    void removeEntity(EID eid);

    template<class ComponentClass, typename... Args>
    ComponentClass* addComponent(Entity *entity, Args... args) {
      ComponentClass* component = new ComponentClass(args...);
      this->components[ComponentClass::cid][entity->eid] = component;
      return component;
    }

    template<class ComponentClass>
    ComponentClass* getComponent(EID eid) {
      if (this->components.find(ComponentClass::cid) == this->components.end()) return nullptr;
      if (this->components[ComponentClass::cid].find(eid) == this->components[ComponentClass::cid].end()) return nullptr;

      auto component = this->components[ComponentClass::cid][eid];

      return static_cast<ComponentClass *>(component);
    }

    template<class ComponentClass>
    vector<EID> getAllEntitiesWithComponent() {
      vector<EID> entities;
      auto components = this->components[ComponentClass::cid];

      for (auto it = components.begin(); it != components.end(); ++it) {
        entities.push_back(it->first);
      }

      return entities;
    }
};

#endif
