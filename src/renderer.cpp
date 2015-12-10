#include "renderer.h"

void Renderer::render() {

  SDL_RenderClear(this->ren);
  auto data = this->data.at("layers").at(4).at("data");

  for (int i = 0; i < data.size(); i++) {
    int element = data[i];

    // Stupid dataset is not 0-based
    SDL_Rect src = {
      ((element - 1) % this->tile_count_w) * this->tile_w,
      ((element - 1) / this->tile_count_w) * this->tile_h,
      this->tile_w,
      this->tile_h
    };

    SDL_Rect dst = {
      (i % this->layer_count_w) * this->tile_w,
      (i / this->layer_count_w) * this->tile_h,
      this->tile_w,
      this->tile_h
    };

    SDL_RenderCopy(this->ren, this->texture, &src, &dst);
  }
  cout << endl;
  SDL_RenderPresent(this->ren);
}
