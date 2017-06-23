#include "collision.h"

void CollisionSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<CollisionComponent>(); 
  auto camera = manager->getComponent<PositionComponent>(manager->getSpecial("camera"));

  for (EID e1 : entities) {
    auto c1 = manager->getComponent<CollisionComponent>(e1);
    auto p1 = manager->getComponent<PositionComponent>(e1);

    if (c1->isStatic) continue;

    c1->collisions.clear();

    for (EID e2 : entities) {
      auto c2 = manager->getComponent<CollisionComponent>(e2);
      auto p2 = manager->getComponent<PositionComponent>(e2);

      if (e1 == e2) {
        p1->y = p1->nextY;
        p1->x = p1->nextX;
        continue;
      }

      int x1 = p1->nextX + c1->x;
      int y1 = p1->nextY + c1->y;
      int x2 = p2->nextX + c2->x;
      int y2 = p2->nextY + c2->y;

      bool collidingX = isOverlapping(x1, x1 + c1->w, x2, x2 + c2->w);
      bool collidingY = isOverlapping(y1, y1 + c1->h, y2, y2 + c2->h);
      bool colliding = collidingX && collidingY;

      c1->isColliding = c1->isColliding || colliding;

      // resolve movements
      if (colliding) {
        c1->collisions[e2] = c2->resolver;
        
        // resolve movements
        int h_distance = abs((x1 + c1->w / 2) - (x2 + c2->w / 2));
        int v_distance = abs((y1 + c1->h / 2) - (y2 + c2->h / 2));

        if (v_distance > h_distance) {
          int overlap = calculateOverlap(y1, y1 + c1->h,  y2, y2 + c2->h);
          int direction = (Compass::NORTH & p1->direction) ? 1 : -1;
          p1->nextY += direction * overlap;
        }
        else if (h_distance > v_distance) {
          int overlap = calculateOverlap(x1, x1 + c1->w, x2, x2 + c2->w);
          int direction = (Compass::EAST & p1->direction) ? -1 : 1;
          p1->nextX += direction * overlap;
        }
      }

      p1->y = p1->nextY;
      p1->x = p1->nextX;
    }
  }
};
