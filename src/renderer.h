#ifndef RENDERER_H
#define RENDERER_H

#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include "json.hpp"
#include "readjson.h"

using json = nlohmann::json;
using namespace std;

struct renderer_error { };

class Renderer {

  protected:
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *texture = nullptr;

    json data;

    int tile_w;
    int tile_h;

    int tile_count_w;
    int tile_count_h;

    int layer_count_w;
    int layer_count_h;

  public:
    Renderer(string levelFile) {
      this->data = readFile(levelFile.c_str());

      if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw renderer_error();
      }

      // creating a window
      this->win = SDL_CreateWindow(
        "Game",
        0,
        0,
        640,
        480,
        SDL_WINDOW_SHOWN
      );
      if (this->win == nullptr) {
        std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw renderer_error();
      }

      // creating a renderer
      this->ren = SDL_CreateRenderer(
        this->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
      );
      if (this->ren == nullptr) {
        SDL_DestroyWindow(this->win);
        std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw renderer_error();
      }


      if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        std::cout << "IMG_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw renderer_error();
      }

      //TileLayer T(data, "foreground");
      string tileset_file = this->data
        .at("tilesets")
        .at(0)
        .at("image")
        .get<string>().c_str();

      string base_path = string(SDL_GetBasePath());
      string tileset_path = base_path + "assets/" + tileset_file;

      cout << tileset_path << '\n';
      this->texture = IMG_LoadTexture(this->ren, tileset_path.c_str());
      if (this->texture == nullptr){
        std::cout << "LoadTexture Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        throw renderer_error();
      }

      this->tile_w = this->data.at("tilewidth").get<int>();
      this->tile_h = this->data.at("tileheight").get<int>();

      this->tile_count_w = this->data
        .at("tilesets")
        .at(0)
        .at("tilewidth")
        .get<int>();
      this->tile_count_h = this->data
        .at("tilesets")
        .at(0)
        .at("tileheight")
        .get<int>();

      this->layer_count_w = this->data.at("width").get<int>();
      this->layer_count_h = this->data.at("height").get<int>();

    };

    ~Renderer() {
      SDL_DestroyRenderer(this->ren);
      SDL_DestroyWindow(this->win);
    }

    void render();
};

#endif
