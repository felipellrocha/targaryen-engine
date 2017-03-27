#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include <stdexcept>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "json/json.h"
#include "exceptions.h"
#include "renderer/node.h"

using json = nlohmann::json;
using namespace std;

class ImageLayer : public Node {

  protected:
    int x, y, w, h, total_width;
    string tiling;

  public:
    ImageLayer(SDL_Renderer *renderer, json data, int layer);
    void render() override;
    void update() override;
    void input(SDL_Event event) override;
};

#endif
