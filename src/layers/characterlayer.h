#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdexcept>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/node.h"
#include "renderer/orientation.h"

#include "states/basestate.h"
#include "states/idling.h"

#include "physics/AABB.h"
#include "physics/IAABB.h"

using json = nlohmann::json;

using namespace std;

class CharacterLayer : public enable_shared_from_this<CharacterLayer>, public Node, public IAABB {
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
