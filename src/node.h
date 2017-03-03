#ifndef NODE_H
#define NODE_H

#include <SDL2/SDL.h>

class Node {
  public:
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;

    virtual void render() =0;
    virtual void update() =0;
    virtual void input(SDL_Event event) =0;
    virtual ~Node() { };
};

#endif

