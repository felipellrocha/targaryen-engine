#ifndef SURROUNDING_H
#define SURROUNDING_H

enum TileSurrounding {
  NONE = 0,
  NORTH = 1,
  NORTHEAST = 2,
  EAST = 4,
  SOUTHEAST = 8,
  SOUTH = 16,
  SOUTHWEST = 32,
  WEST = 64,
  NORTHWEST = 128,
};

#endif
