#ifndef IDLING_H
#define IDLING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "../characterlayer.h"

class CharacterLayer;

class IdlingState : public BaseState {
  private:
    int frame_index = 0;
    int x, y, h, w;

    CharacterLayer * parent;

  public:
    IdlingState(CharacterLayer * parent);

    void render() override;
    void animate() override;

    ~IdlingState() { };
};

#endif
