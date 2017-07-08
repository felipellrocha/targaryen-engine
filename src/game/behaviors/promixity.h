#ifndef PROXIMITY_H
#define PROXIMITY_H

#include "AI/leaf.h"

class ProximityBehavior : public Leaf {
public:
  Status update() {
    cout << "close!" << endl;
  };
};

#endif
