#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  auto cameraPosition = manager->getComponent<PositionComponent>(manager->getCamera());
  auto cameraDimension = manager->getComponent<DimensionComponent>(manager->getCamera());

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

#ifdef DRAW_COLLISION
      auto collision = manager->getComponent<CollisionComponent>(entity);

      if (!collision) {
        continue;
      }

			// Create a rectangle
			SDL_Rect r;
			r.x = position->x - cameraPosition->x;
			r.y = position->y - cameraPosition->y;
			r.w = sprite->w;
			r.h = sprite->h;
			 
			SDL_SetRenderDrawColor( game->ren, 100, 255, 0, 0 );
			 
			SDL_RenderDrawRect( game->ren, &r );
#endif
    }
    else {
      grid->layer.render(
        cameraPosition->x,
        cameraPosition->y,
        cameraDimension->w,
        cameraDimension->h
      );
    }
  }
#ifdef DRAW_COLLISION

  SDL_Rect r;
  r.x = 0;
  r.y = 0;
  r.w = cameraDimension->w;
  r.h = cameraDimension->h;
   
  SDL_SetRenderDrawColor( game->ren, 0, 0, 255, 0 );
   
  SDL_RenderDrawRect( game->ren, &r );

  SDL_SetRenderDrawColor( game->ren, 0, 0, 0, 255 );
#endif
};
