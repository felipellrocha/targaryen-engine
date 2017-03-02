#ifndef IDLING_H
#define IDLING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "../characterlayer.h"
#include "../orientation.h"

class CharacterLayer;

class IdlingState : public BaseState {
  private:
    int frame_index = 0;
    int x, y, h, w = 0;

    CharacterLayer * parent;

  public:
    IdlingState(CharacterLayer * parent);

    void render() override;
    void animate() override;
    void input(SDL_Event event) override;

    ~IdlingState() { };
};

#endif
