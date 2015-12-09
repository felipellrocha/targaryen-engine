#include "renderer.h"

void Renderer::render() {

    auto data = this->data.at("layers").at(4).at("data");

    SDL_RenderClear(this->ren);
    /*
    for (int i = 0; i < data.size(); i++) {
      int element = data[i];

      SDL_Rect src = {
        (element / this->tile_count_w) * this->tile_w,
        (element % this->tile_count_w) * this->tile_h,
        this->tile_w,
        this->tile_h
      };

      SDL_Rect dst = {
        (i / this->layer_count_w) * this->tile_w,
        (i % this->layer_count_w) * this->tile_h,
        this->tile_w,
        this->tile_h
      };

      SDL_RenderCopy(this->ren, this->texture, &src, &dst);
    }
    */
    SDL_Rect dst = { 0, 0, 100, 100 };
    SDL_RenderCopy(this->ren, this->texture, NULL, &dst);
    SDL_RenderPresent(this->ren);
    SDL_Delay(1000);
}
