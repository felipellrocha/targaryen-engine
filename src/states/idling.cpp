#include "idling.h"

IdlingState::IdlingState(CharacterLayer * parent) {
  this->parent = parent;
}

void IdlingState::render() {

  try {
    // that to_string just *cannot* be efficient... Alas...
    auto frame = this->parent->data.at("animations").at("idling").at("keyframes").at(to_string(frame_index));

    this->x = frame.at(0).get<int>();
    this->y = frame.at(1).get<int>();
    this->w = frame.at(2).get<int>();
    this->h = frame.at(3).get<int>();
  } catch (const out_of_range &err) { }

  SDL_Rect src = { this->x, this->y, this->w, this->h };
  SDL_Rect pos = { this->parent->x, this->parent->y, this->w, this->h };
  SDL_RenderCopy(this->parent->renderer, this->parent->texture, &src, &pos);
}

void IdlingState::animate() {
    int total_frames = this->parent->data
      .at("animations")
      .at("idling")
      .at("total_frames").get<int>();
    frame_index = (frame_index + 1) % total_frames;
}
