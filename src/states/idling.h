#ifndef IDLING_H
#define IDLING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "running.h"
#include "layers/characterlayer.h"
#include "renderer/orientation.h"

using namespace std;

class CharacterLayer;

class IdlingState : public BaseState {
  private:
    int frame_index = -1;
    int x, y, h, w = 0;

    CharacterLayer * parent;

  public:
    IdlingState(CharacterLayer * parent);

    void setPosition();
    void render() override;
    BaseState* update() override;
    BaseState* input(SDL_Event event) override;

    ~IdlingState() { };
};

#endif
