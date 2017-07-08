#ifndef SYSTEMAI_H
#define SYSTEMAI_H

#include "sdl2image.h"

#include "game/components.h"

#include "renderer/renderer.h"

#include "entity/entity.h"
#include "entity/system.h"

#include "AI/behaviortree.h"
//#include "AI/composite.h"
#include "AI/decorator.h"
#include "AI/node.h"
#include "AI/leaf.h"

#include "game/behaviors/follow.h"

using namespace std;

class AISystem : public System {
public:
  BehaviorTree* tree;

  void update(float dt);

  AISystem(EntityManager *_manager, Renderer *_game) :
  System(_manager, _game) {
    tree = new BehaviorTree(_manager, _game);

    auto follower = new FollowerBehavior();
    auto repeater = new Limiter(5);

    repeater->setChild(follower);

    tree->setRoot(repeater);
  };

  ~AISystem() {
    delete tree;
  }
};

#endif
