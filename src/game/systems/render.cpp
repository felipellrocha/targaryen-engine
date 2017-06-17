#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  auto cameraPosition = manager->getComponent<PositionComponent>(manager->getSpecial("camera"));
  auto cameraDimension = manager->getComponent<DimensionComponent>(manager->getSpecial("camera"));

  for (int i = 0; i < entities.size(); i++) {
    EID entity = entities[i];

    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto grid = manager->getComponent<GridComponent>(entity);

    if (sprite) {
      SDL_Rect src = {
        sprite->x,
        sprite->y,
        sprite->w,
        sprite->h
      };

      SDL_Rect dst = {
        position->x - cameraPosition->x,
        position->y - cameraPosition->y,
        sprite->w,
        sprite->h
      };
			SDL_RenderCopy(game->ren, sprite->texture, &src, &dst);

    }
    else {
      grid->layer.render(
        cameraPosition->x,
        cameraPosition->y,
        cameraDimension->w,
        cameraDimension->h
      );
    }

#ifdef DRAW_COLLISION
    auto collision = manager->getComponent<CollisionComponent>(entity);
    if (!collision) continue;

    int x = position->nextX + collision->x;
    int y = position->nextY + collision->y;

    // Create a rectangle
    SDL_Rect r;
    r.x = x - cameraPosition->x;
    r.y = y - cameraPosition->y;
    r.w = collision->w;
    r.h = collision->h;
     
    SDL_SetRenderDrawColor( game->ren, 100, 255, 0, 0 );
     
    SDL_RenderDrawRect( game->ren, &r );
#endif

  }
};
