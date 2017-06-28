#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <typeinfo>
#include <typeindex>
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
    map<type_index, map<EID, Component *>> components;
    map<string, EID> special;

    EID getSpecial(string key);
    void saveSpecial(string key, EID entity);
    void saveSpecial(string key, Entity *entity);

    EID generateEid();
    Entity* createEntity();
    void removeEntity(EID eid);

    template<class ComponentClass, typename... Args>
    ComponentClass* addComponent(EID entity, Args... args) {
      ComponentClass* component = new ComponentClass(args...);
      type_index cid = type_index(typeid(ComponentClass));
      this->components[cid][entity] = component;
      return component;
    }

    template<class ComponentClass, typename... Args>
    ComponentClass* addComponent(Entity *entity, Args... args) {
      return this->addComponent<ComponentClass>(entity->eid, args...);
    }

    template<class ComponentClass>
    void removeComponent(EID eid) {
      type_index cid = type_index(typeid(ComponentClass));
      if (this->components[cid].find(eid) == this->components[cid].end()) return;

      this->components[cid].erase(eid);
    }

    template<class ComponentClass>
    ComponentClass* getComponent(EID eid) {
      type_index cid = type_index(typeid(ComponentClass));
      if (this->components.find(cid) == this->components.end()) return nullptr;
      if (this->components[cid].find(eid) == this->components[cid].end()) return nullptr;

      auto component = this->components[cid][eid];

      return static_cast<ComponentClass *>(component);
    }

    template<class ComponentClass>
    vector<EID> getAllEntitiesWithComponent() {
      vector<EID> entities;
      type_index cid = type_index(typeid(ComponentClass));
      auto components = this->components[cid];

      for (auto it = components.begin(); it != components.end(); ++it) {
        entities.push_back(it->first);
      }

      return entities;
    }
};

#endif
