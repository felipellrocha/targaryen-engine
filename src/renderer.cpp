#include "renderer.h"

void Renderer::render(SDL_Renderer *ren) {
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderPresent(ren);
}
