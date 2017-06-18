#include "collision.h"

void CollisionSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<CollisionComponent>(); 
  auto camera = manager->getComponent<PositionComponent>(manager->getSpecial("camera"));
  for (int i = 0; i < entities.size(); i++) {
    EID e1 = entities[i];
    auto c1 = manager->getComponent<CollisionComponent>(e1);
    auto p1 = manager->getComponent<PositionComponent>(e1);

    if (c1->isStatic) continue;

    c1->collisions.clear();

    int x1 = p1->nextX + c1->x;
    int y1 = p1->nextY + c1->y;

    for (int j = 0; j < entities.size(); j++) {
      EID e2 = entities[j];

      auto c2 = manager->getComponent<CollisionComponent>(e2);
      auto p2 = manager->getComponent<PositionComponent>(e2);

      if (e1 == e2) {
        p1->y = p1->nextY;
        p1->x = p1->nextX;
        continue;
      }

      int x2 = p2->nextX + c2->x;
      int y2 = p2->nextY + c2->y;

      bool colliding = 
        (overlap(x1, x1 + c1->w, x2, x2 + c2->w) &&
        overlap(y1, y1 + c1->h, y2, y2 + c2->h));

      c1->isColliding = c1->isColliding || colliding;

      // resolve movements
      if (colliding) {
        c1->collisions[e2] = c2->resolver;
        
        // resolve movements
        int h_distance = abs((x1 + c1->w / 2) - (x2 + c2->w / 2));
        int v_distance = abs((y1 + c1->h / 2) - (y2 + c2->h / 2));

        if (v_distance > h_distance) {
          int offset = abs(v_distance - (c1->h / 2) - (c2->h / 2));
          int direction = (Compass::NORTH & p1->direction) ? 1 : -1;
          p1->nextY += direction * offset;
        }
        else if (h_distance > v_distance) {
          int offset = abs(h_distance - (c1->w / 2) - (c2->w / 2));
          int direction = (Compass::EAST & p1->direction) ? -1 : 1;
          p1->nextX += direction * offset;
        }
      }

      p1->y = p1->nextY;
      p1->x = p1->nextX;

#ifdef DRAW_COLLISION
      if (colliding) {
        SDL_RenderDrawLine(
          game->ren,
          x1 + (c1->w / 2) - camera->x,
          y1 + (c1->h / 2) - camera->y,
          x2 + (c2->w / 2) - camera->x,
          y2 + (c2->h / 2) - camera->y
        );
      }
#endif
    }
  }
};
