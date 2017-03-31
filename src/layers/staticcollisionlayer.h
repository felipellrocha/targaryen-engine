#ifndef STATICCOLLISIONLAYER_H
#define STATICCOLLISIONLAYER_H

#include <SDL2/SDL.h>
#include "sdl2image.h"
#include <string>
#include <vector>
#include <memory>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/node.h"

#include "physics/AABB.h"
#include "physics/AABBTree.h"

using json = nlohmann::json;
using namespace std;

class StaticCollisionLayer : public enable_shared_from_this<StaticCollisionLayer>, public Node, public IAABB {

  protected:
    vector<int> layers;
    int x, y, w, h;
    AABB aabb;

  public:
    StaticCollisionLayer(SDL_Renderer *renderer, json data); //, int index, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;

    AABB getAABB() const override { return aabb; }
};

#endif
