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
    vector<EID> entities;
    map<EID, map<CID, Component *>> components;
		map<CID, vector<EID>> cache;

    EID camera;
    EID player;

    EID generateEid();
    Entity* createEntity();

    EID getCamera() {
      return this->camera;
    }

    void savePlayer(Entity *player) {
      this->player = player->eid;
    }

    void saveCamera(Entity *camera) {
      this->camera = camera->eid;
    }

    template<class ComponentClass, typename... Args> void addComponent(Entity *entity, Args... args) {
      ComponentClass *component = new ComponentClass(args...);
      this->cache[ComponentClass::cid].push_back(entity->eid);
      this->components[ComponentClass::cid][entity->eid] = component;
    }

    template<class ComponentClass> ComponentClass* getComponent(EID eid)
    {
      return static_cast<ComponentClass *>(this->components[ComponentClass::cid][eid]);
    }

    template<class ComponentClass>
    vector<EID> getAllEntitiesWithComponent()
    {
			return this->cache[ComponentClass::cid];
    }
};

#endif
