#include "tilelayer.h"
#include <typeinfo>

TileLayer::TileLayer(GPU_Target *renderer, vector<Tileset *> tilesets, json game_data, json map_data, int layer) {

  this->renderer = renderer;
  this->tilesets = tilesets;

  this->tile_w = game_data.at("tile").at("width").get<int>();
  this->tile_h = game_data.at("tile").at("height").get<int>();

  int columns = map_data.at("grid").at("columns").get<int>();
  int rows = map_data.at("grid").at("rows").get<int>();
  this->grid = new Grid(rows, columns);

  auto tiles = map_data.at("layers").at(layer).at("data");
  for (int locationIndex = 0; locationIndex < tiles.size(); locationIndex++) {
    auto t = tiles.at(locationIndex);
    int setIndex = t.at(0).get<int>();
    int tileIndex = t.at(1).get<int>();

    Tile *tile = new Tile(setIndex, tileIndex, locationIndex);
    this->tiles.push_back(tile);
  }

  for (int i = 0; i < this->tiles.size(); i++) {
    Tile *tile = this->tiles[i];
    Tileset *tileset = this->tilesets[tile->setIndex];

    if (tile->setIndex < 0) continue;

    if (tileset->type == "aware") tile->surrounding = this->findSurroundings(tile, this->grid);
  }
}

void TileLayer::render(int x, int y, int w, int h) {
  for (int i = 0; i < this->tiles.size(); i++) {
    Tile *t = this->tiles[i];

    if (t->setIndex < 0) continue;

    int tileX = this->grid->getX(i) * this->tile_w;
    int tileY = this->grid->getY(i) * this->tile_h;

    bool colliding = 
      overlap(tileX, tileX + this->tile_w, x, x + w) &&
      overlap(tileY, tileY + this->tile_h, y, y + h);

    if (!colliding) continue;

    Tileset *tileset = tilesets[t->setIndex];

    if (tileset->type == "aware") {
      string terrain = tileset->terrains[t->tileIndex];
      if (terrain == "4-tile") {
        FourTile *tile = static_cast<FourTile *>(t);
        tile->render(tileset, this->grid, this->renderer, this->tile_w, this->tile_h, -x, -y);
      }
      else {
        SixTile *tile = static_cast<SixTile *>(t);
        tile->render(tileset, this->grid, this->renderer, this->tile_w, this->tile_h, -x, -y);
      }
    }
    else {
      Tile *tile = t;
      tile->render(tileset, this->grid, this->renderer, this->tile_w, this->tile_h, -x, -y);
    }
  }
}

int TileLayer::findSurroundings(Tile *t1, Grid *grid) {
  int x = grid->getX(t1->locationIndex);
  int y = grid->getY(t1->locationIndex);

  int surrounding = 0;

  Tile *t2;
  int index;

  index = grid->getIndex(x, y - 1);
  if (grid->isInside(x, y - 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::NORTH;
    }
  }

  index = grid->getIndex(x + 1, y - 1);
  if (grid->isInside(x + 1, y - 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::NORTHEAST;
    }
  }

  index = grid->getIndex(x + 1, y);
  if (grid->isInside(x + 1, y)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::EAST;
    }
  }

  index = grid->getIndex(x + 1, y + 1);
  if (grid->isInside(x + 1, y + 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::SOUTHEAST;
    }
  }

  index = grid->getIndex(x, y + 1);
  if (grid->isInside(x, y + 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::SOUTH;
    }
  }

  index = grid->getIndex(x - 1, y + 1);
  if (grid->isInside(x - 1, y + 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::SOUTHWEST;
    }
  }

  index = grid->getIndex(x - 1, y);
  if (grid->isInside(x - 1, y)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::WEST;
    }
  }

  index = grid->getIndex(x - 1, y - 1);
  if (grid->isInside(x - 1, y - 1)) {
    t2 = this->tiles[index];
    if (t1->setIndex == t2->setIndex && t1->tileIndex == t2->tileIndex) {
      surrounding += Compass::NORTHWEST;
    }
  }

  return surrounding;
}
