#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

class Renderer {

  protected:
    //Document data;

  public:
    /*
    Renderer(Document &d) {
      data = d;
    };
    */

    void render(SDL_Renderer *ren);
};

#endif
