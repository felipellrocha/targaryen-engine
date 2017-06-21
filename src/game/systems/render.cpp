#include "render.h"

void RenderSystem::update(float dt) {
  auto entities = manager->getAllEntitiesWithComponent<RenderComponent>(); 
  auto cameraPosition = manager->getComponent<PositionComponent>(manager->getSpecial("camera"));
  auto cameraDimension = manager->getComponent<DimensionComponent>(manager->getSpecial("camera"));

  SDL_Rect camera = {
    cameraPosition->x,
    cameraPosition->y,
    cameraDimension->w,
    cameraDimension->h,
  };

  for (EID entity : entities) {
    auto tile = manager->getComponent<TileComponent>(entity);
    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);

    if (tile) {
      int tileX = game->grid.getX(tile->locationIndex) * game->grid.tile_w;
      int tileY = game->grid.getY(tile->locationIndex) * game->grid.tile_h;

      bool colliding = 
        overlap(tileX, tileX + game->grid.tile_w, camera.x, camera.x + camera.w) &&
        overlap(tileY, tileY + game->grid.tile_h, camera.y, camera.y + camera.h);

      if (!colliding) continue;

      Tileset *tileset = game->tilesets[tile->setIndex];
      vector<array<SDL_Rect, 2>> sources;
      if (tileset->type == "tile") {
         sources = simpleTile::calculateAll(tile, tileset, game->grid); 
      }
      else if (tileset->terrains[tile->tileIndex] == "6-tile") {
         sources = sixTile::calculateAll(tile, tileset, game->grid); 
      } else if (tileset->terrains[tile->tileIndex] == "4-tile") {
         sources = fourTile::calculateAll(tile, tileset, game->grid); 
      }

      for (auto calc : sources) {

        SDL_Rect src = calc[0];
        SDL_Rect dst = calc[1];

        dst.x -= camera.x;
        dst.y -= camera.y;

        SDL_RenderCopy(game->ren, tileset->texture, &src, &dst);
      }

    }
    else if (sprite) {
      SDL_Rect src = {
        sprite->x,
        sprite->y,
        sprite->w,
        sprite->h
      };

      SDL_Rect dst = {
        position->x - camera.x,
        position->y - camera.y,
        sprite->w,
        sprite->h
      };
			SDL_RenderCopy(game->ren, sprite->texture, &src, &dst);
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
     
    SDL_SetRenderDrawColor( game->ren, 100, 255, 0, 250 );
     
    SDL_RenderDrawRect( game->ren, &r );
#endif

  }

  SDL_SetRenderDrawColor( game->ren, 0, 0, 0, 0 );
};
