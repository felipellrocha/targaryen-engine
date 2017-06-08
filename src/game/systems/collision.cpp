#include "collision.h"

void CollisionSystem::update(float dt) {
  vector<EID> entities = manager->getAllEntitiesWithComponent<CollisionComponent>(); 
  for (int i = 0; i < entities.size(); i++) {
    EID e1 = entities[i];
    auto c1 = manager->getComponent<CollisionComponent>(e1);
    auto p1 = manager->getComponent<PositionComponent>(e1);
    auto d1 = manager->getComponent<DimensionComponent>(e1);

    if (c1->isStatic) continue;

    for (int j = 0; j < entities.size(); j++) {
      EID e2 = entities[j];

      auto p2 = manager->getComponent<PositionComponent>(e2);
      auto d2 = manager->getComponent<DimensionComponent>(e2);


			if (e1 == e2) {
        p1->y = p1->nextY;
        p1->x = p1->nextX;
        continue;
      }

      bool colliding = 
        overlap(p1->nextX, p1->nextX + d1->w, p2->nextX, p2->nextX + d2->w) &&
        overlap(p1->nextY, p1->nextY + d1->h, p2->nextY, p2->nextY + d2->h);

      int h_distance = abs((p1->nextX + d1->w / 2) - (p2->nextX + d2->w / 2));
      int v_distance = abs((p1->nextY + d1->h / 2) - (p2->nextY + d2->h / 2));

      // resolve movements
      if (colliding) {

        if (v_distance > h_distance) {
          int offset = abs(v_distance - (d1->h / 2) - (d2->h / 2));
          int direction = (Compass::NORTH & p1->direction) ? 1 : -1;
          p1->nextY += direction * offset;
        }
        else if (h_distance > v_distance) {
          int offset = abs(h_distance - (d1->w / 2) - (d2->w / 2));
          int direction = (Compass::EAST & p1->direction) ? -1 : 1;
          p1->nextX += direction * offset;

        }
      }

      p1->y = p1->nextY;
      p1->x = p1->nextX;

      c1->isColliding = colliding;

/*
#ifdef DRAW_COLLISION
			if (colliding) {
        SDL_RenderDrawLine(
          game->ren,
          (p1->x + p1->x + d1->w) / 2 - camera->x,
          (p1->y + p1->y + d1->h) / 2 - camera->y,
          (p2->x + p2->x + d2->w) / 2 - camera->x,
          (p2->y + p2->y + d2->h) / 2 - camera->y
        );
	    }
#endif
*/
    }
  }
};
