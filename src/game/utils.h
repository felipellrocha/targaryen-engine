#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <math.h>
#include "entity/entity.h"

enum Resolver {
  OPAQUE = 1,
  EXPLOSION = 2,
};

struct Collision {
  public:
    EID e1;
    EID e2;

    int r1;
    int r2;

    Collision (EID _e1, EID _e2, int _r1, int _r2)
      : e1(_e1), e2(_e2), r1(_r1), r2(_r2) { };

    bool operator< (Collision const &other) const {
      EID e1l = min(e1, e2);
      EID e1h = max(e1, e2);
      EID e2l = min(other.e1, other.e2);
      EID e2h = max(other.e1, other.e2);

      int r1l = min(r1, r2);
      int r1h = max(r1, r2);
      int r2l = min(other.r1, other.r2);
      int r2h = max(other.r1, other.r2);
     
      return e1l < e2l || e1h < e2h || r1l < r2l || r1h < r2h;
    }
};

bool overlap(int a0, int a1, int b0, int b1);


#endif
