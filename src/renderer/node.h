#ifndef NODE_H
#define NODE_H

#include <SDL2/SDL.h>
#include <memory>
#include "physics/AABBTree.h"

using namespace std;

class Node {
  public:
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;

    vector<shared_ptr<Node>> collisions;

    virtual void render() =0;
    virtual void update() =0;
    virtual void input(SDL_Event event) =0;
    virtual ~Node() { };
};

#endif

