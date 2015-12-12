#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include <stdexcept>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "json.hpp"
#include "exceptions.h"
#include "node.h"

using json = nlohmann::json;
using namespace std;

class ImageLayer : public Node {

  protected:
    int x, y, w, h;
    string tiling;

  public:
    ImageLayer(SDL_Renderer *renderer, json data, int layer);
    void render() override;
};

#endif
