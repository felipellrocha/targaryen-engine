#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdexcept>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "json.hpp"
#include "exceptions.h"
#include "node.h"
#include "orientation.h"

#include "states/basestate.h"
#include "states/idling.h"

using json = nlohmann::json;
using namespace std;

class CharacterLayer : public Node {
  public:
    BaseState * state;
    json data;

    int x = 160;
    int y = 280;

    Orientation orientation = Orientation::RIGHT;

    CharacterLayer(SDL_Renderer *renderer, json data_path);
    void render() override;
    void input(SDL_Event event) override;
};

#endif
