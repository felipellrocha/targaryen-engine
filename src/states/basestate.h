#ifndef BASESTATE_H
#define BASESTATE_H

#include <SDL2/SDL.h>

class BaseState {
  public:
    virtual ~BaseState() { };
    virtual void render() =0;
    virtual BaseState* input(SDL_Event event) =0;

    virtual BaseState* update() {
      return NULL;
    };
};

#endif


