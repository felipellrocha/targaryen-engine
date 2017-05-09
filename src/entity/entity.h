#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef uint32_t EID;
typedef uint32_t CID;

struct Entity {
  EID eid;

  Entity(EID _eid) : eid(_eid) {}
};

struct Component {
  static CID cid;

  CID getCID() { return cid; }
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

class System {
  public:
    EntityManager manager;

    System(EntityManager _manager) : manager(_manager) {};
    virtual void update(float dt) =0;
};

#endif
