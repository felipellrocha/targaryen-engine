#ifndef IDLING_H
#define IDLING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "running.h"
#include "jumping.h"
#include "layers/characterlayer.h"
#include "renderer/orientation.h"

using namespace std;

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
    BaseState* update() override;
    BaseState* input(SDL_Event event) override;

    ~IdlingState() { };
};

#endif
