#ifndef LEAF_H
#define LEAF_H

class Leaf : public Node {
public:
  Leaf() {}
  
  virtual Status update() =0;
};

#endif
