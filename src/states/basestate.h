#ifndef BASESTATE_H
#define BASESTATE_H

#include <SDL2/SDL.h>

class BaseState {
  public:
    virtual ~BaseState() { };
    virtual void render() =0;
    virtual void animate() =0;
    virtual void input(SDL_Event event) =0;
};

#endif


