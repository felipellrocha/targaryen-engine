#ifndef NODE_H
#define NODE_H

#include <SDL2/SDL.h>

class Node {
  protected:
    SDL_Texture *image;

  public:
    virtual void render() =0;
    virtual ~Node() { };
};

#endif

