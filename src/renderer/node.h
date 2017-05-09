#ifndef NODE_H
#define NODE_H

#include <SDL2/SDL.h>
#include <memory>
#include "physics/AABBTree.h"
#include "renderer/tileset.h"

using namespace std;

class Node {
  public:
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;

    vector<Tileset *> tilesets;

    vector<shared_ptr<Node>> collisions;

    virtual void render() =0;
    virtual void update() =0;
    virtual void input(SDL_Event event) =0;
    virtual ~Node() { };
};

#endif

