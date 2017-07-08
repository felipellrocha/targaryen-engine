#ifndef BEHAVIORTREE_H
#define BEHAVIORTREE_H

#include "node.h"
#include "entity/entity.h"
#include "renderer/renderer.h"

class BehaviorTree : public Node {
public:
  Node* root = nullptr;
  EntityManager* manager;
  Renderer* game;

  BehaviorTree(EntityManager* _manager, Renderer* _game) :
    manager(_manager), game(_game) { }

  Status update() { return root->tick(); }
  void setRoot (Node* node) { root = node; }

  ~BehaviorTree() {
    delete root;
    root = nullptr;
  }
};

#endif
