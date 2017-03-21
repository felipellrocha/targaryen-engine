#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdexcept>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "json/json.hpp"
#include "exceptions.h"
#include "renderer/node.h"
#include "renderer/orientation.h"

#include "states/basestate.h"
#include "states/idling.h"

#include "physics/IAABB.h"
#include "physics/AABB.h"

using json = nlohmann::json;
using namespace std;

class CharacterLayer : public Node, public IAABB {
  public:
    BaseState * state = NULL;
    json data;

    int x = 160;
    int y = 280;

    AABB aabb;

    Orientation orientation = Orientation::RIGHT;

    CharacterLayer(SDL_Renderer *renderer, json data_path);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;

    AABB getAABB() const override { return aabb; }
};

#endif
