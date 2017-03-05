#ifndef JUMPING_H
#define JUMPING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "idling.h"
#include "characterlayer.h"
#include "orientation.h"

class CharacterLayer;

class JumpingState : public BaseState {
  private:
    int x, y, h, w = 0;
    CharacterLayer * parent;

  public:
    JumpingState(CharacterLayer * parent);

    void render() override;
    void animate() override;
    BaseState* update() override;
    BaseState* input(SDL_Event event) override;

    ~JumpingState() { };
};

#endif
