#include "walk.h"

void WalkSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<WalkComponent>(); 
  Animation animation;

  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto walk = manager->getComponent<WalkComponent>(entity);

    if (Compass::NORTH & walk->direction) {
      animation = this->game->animations["walk-north"];
      walk->direction = Compass::NORTH;
    }
    else if (Compass::SOUTH & walk->direction) {
      animation = this->game->animations["walk-south"];
      walk->direction = Compass::SOUTH;
    }
    else if (Compass::EAST & walk->direction) {
      animation = this->game->animations["walk-east"];
      walk->direction = Compass::EAST;
    }
    else if (Compass::WEST & walk->direction) {
      animation = this->game->animations["walk-west"];
      walk->direction = Compass::WEST;
    }
    else {
      animation = this->game->animations["walk-south"];
      walk->direction = Compass::SOUTH;
    }

    walk->frame = (walk->animating) ?
      (walk->frame + 1) % animation.numberOfFrames :
      0;

    if (!animation.keyframes.count(walk->frame)) continue;

    SDL_Rect keyframe = animation.keyframes[walk->frame];

    sprite->x = keyframe.x;
    sprite->y = keyframe.y;
    sprite->w = keyframe.w;
    sprite->h = keyframe.h;
  }
};

