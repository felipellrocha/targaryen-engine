#ifndef RUNNING_H
#define RUNNING_H

#include <SDL2/SDL.h>
#include <stdexcept>
#include "basestate.h"
#include "idling.h"
#include "layers/characterlayer.h"
#include "layers/staticcollisionlayer.h"
#include "layers/itemlayer.h"
#include "renderer/orientation.h"

using namespace std;

class CharacterLayer;

class RunningState : public BaseState {
  private:
    int frame_index = 0;
    int x, y, h, w = 0;

    CharacterLayer * parent;

  public:
    RunningState(CharacterLayer * parent);

    void render() override;
    BaseState* update() override;
    BaseState* input(SDL_Event event) override;

    void moveAnimationFrame();

    ~RunningState() { };
};

#endif
