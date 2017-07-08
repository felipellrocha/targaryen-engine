#ifndef FOLLOW_H
#define FOLLOW_H

#include "AI/leaf.h"

class FollowerBehavior : public Leaf {
public:
  Status update() override {
    cout << "following!" << endl;

    return Status::SUCCESS;
  };
};

#endif
