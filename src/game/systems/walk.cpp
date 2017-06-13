#include "walk.h"

void WalkSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<WalkComponent>(); 
  Animation animation;

  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];
    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto walk = manager->getComponent<WalkComponent>(entity);
    auto movement = manager->getComponent<WalkComponent>(entity);

    int direction = walk->direction;

    if (movement) {
      if (this->game->compass == 0) walk->animating = false;
      else walk->animating = true;
    }

    if (Compass::NORTH & direction) {
      animation = this->game->animations["walk-north"];
    }
    else if (Compass::SOUTH & direction) {
      animation = this->game->animations["walk-south"];
      walk->direction = Compass::SOUTH;
    }
    else if (Compass::EAST & direction) {
      animation = this->game->animations["walk-east"];
      walk->direction = Compass::EAST;
    }
    else if (Compass::WEST & direction) {
      animation = this->game->animations["walk-west"];
      walk->direction = Compass::WEST;
    }
    else {
      animation = this->game->animations["walk-south"];
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

