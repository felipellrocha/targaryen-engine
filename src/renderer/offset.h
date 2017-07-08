#ifndef OFFSET_H
#define OFFSET_H

struct offset {
  int x;
  int y;

  offset(int _x, int _y) : x(_x), y(_y) { };
};

struct dimension {
  int w;
  int h;

  dimension(int _w, int _h) : w(_w), h(_h) { };
};

struct rect {
  int x;
  int y;
  int w;
  int h;

  rect(int _x, int _y, int _w, int _h)
    : x(_x), y(_y), w(_w), h(_h) { };

  rect() : rect(0, 0, 0, 0) { };
};

#endif
