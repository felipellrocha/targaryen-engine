#include "render.h"

bool RenderCacheItem::operator< (RenderCacheItem const &other) const {
  return tie(layer, y, entity) < tie(other.layer, other.y, other.entity);
}

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
    auto render = manager->getComponent<RenderComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);

    int x = position->x;
    int y = position->y;

    bool colliding = 
      isOverlapping(x, x + game->grid.tile_w, camera.x, camera.x + camera.w) &&
      isOverlapping(y, y + game->grid.tile_h, camera.y, camera.y + camera.h);

    if (!colliding) continue;

    RenderCacheItem c = RenderCacheItem(render->layer, position->y, entity);
    cache.insert(c);
  }

  for (auto &cacheItem : cache) {
    EID entity = cacheItem.entity;

    auto tile = manager->getComponent<TileComponent>(entity);
    auto sprite = manager->getComponent<SpriteComponent>(entity);
    auto position = manager->getComponent<PositionComponent>(entity);
    auto dimension = manager->getComponent<DimensionComponent>(entity);
    auto health = manager->getComponent<HealthComponent>(entity);
    auto color = manager->getComponent<ColorComponent>(entity);

    if (color) {
      SDL_SetRenderDrawColor( game->ren, color->r, color->g, color->b, color->a );
      SDL_Rect r;
      r.x = position->x;
      r.y = position->y;
      r.w = dimension->w;
      r.h = dimension->h;
      SDL_RenderFillRect(game->ren, &r);
    }

    if (tile) {
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

    if (health) {
      string healthDisplay = to_string(health->hearts)
        + "/"
        + to_string(health->max);

      bgSurface = TTF_RenderText_Blended(outline, healthDisplay.c_str(), black);
      fgSurface = TTF_RenderText_Blended(font, healthDisplay.c_str(), white);
      message = SDL_CreateTextureFromSurface(game->ren, bgSurface);

      SDL_Rect rect; 
      rect.w = bgSurface->w;
      rect.h = bgSurface->h;
      rect.x = position->x + (dimension->w / 2) - (rect.w / 2) - camera.x;
      rect.y = position->y - 17 - camera.y;

      SDL_RenderCopy(game->ren, message, NULL, &rect);

      message = SDL_CreateTextureFromSurface(game->ren, fgSurface);

      rect.w = fgSurface->w;
      rect.h = fgSurface->h;
      rect.x = position->x + (dimension->w / 2) - (rect.w / 2) - camera.x;
      rect.y = position->y - 15 - camera.y;

      SDL_RenderCopy(game->ren, message, NULL, &rect);
    }
  }

  #ifdef DRAW_COLLISION
  entities = manager->getAllEntitiesWithComponent<CollisionComponent>(); 
  for (EID entity : entities) {
    auto position = manager->getComponent<PositionComponent>(entity);
    auto collision = manager->getComponent<CollisionComponent>(entity);

    int x = position->x + collision->x;
    int y = position->y + collision->y;

    // Create a rectangle
    SDL_Rect r;
    r.x = x - camera.x;
    r.y = y - camera.y;
    r.w = collision->w;
    r.h = collision->h;
     
    SDL_SetRenderDrawColor( game->ren, 100, 255, 0, 250 );
     
    SDL_RenderDrawRect( game->ren, &r );

    for (auto it : collision->collisions) {
      EID coll = it.first;
      auto c2 = manager->getComponent<CollisionComponent>(coll);
      auto p2 = manager->getComponent<PositionComponent>(coll);

      SDL_Rect r;
      r.x = p2->x + c2->x - camera.x;
      r.y = p2->y + c2->y - camera.y;
      r.w = c2->w;
      r.h = c2->h;
       
      SDL_SetRenderDrawColor( game->ren, 100, 255, 0, 250 );
       
      SDL_RenderDrawRect( game->ren, &r );

      SDL_RenderDrawLine(
        game->ren,
        position->x + collision->x + (collision->w / 2) - camera.x,
        position->y + collision->y + (collision->h / 2) - camera.y,
        p2->x + c2->x + (c2->w / 2) - camera.x,
        p2->y + c2->y + (c2->h / 2) - camera.y
      );
    }
  }
  #endif

#ifdef DRAW_FPS
	stringstream ss;
	ss << fixed << setprecision(2) << (1 / dt);
	string fpsString = ss.str();

	fgSurface = TTF_RenderText_Blended(font, fpsString.c_str(), white);

  message = SDL_CreateTextureFromSurface(game->ren, fgSurface);

  SDL_Rect rect;
  rect.w = fgSurface->w;
  rect.h = fgSurface->h;
  rect.x = 2;
  rect.y = 2;

  SDL_RenderCopy(game->ren, message, NULL, &rect);
#endif

  cache.clear();
  SDL_SetRenderDrawColor( game->ren, 0, 0, 0, 0 );
};
