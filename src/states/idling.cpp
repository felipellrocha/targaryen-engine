#include "idling.h"

IdlingState::IdlingState(CharacterLayer * parent) {
  this->parent = parent;
}

void IdlingState::render() {

  try {
    // that to_string just *cannot* be efficient... Alas...
    auto frame = this->parent->data
      .at("animations")
      .at("idling")
      .at("keyframes")
      .at(to_string(frame_index));

    this->x = frame.at(0).get<int>();
    this->y = frame.at(1).get<int>();
    this->w = frame.at(2).get<int>();
    this->h = frame.at(3).get<int>();
  } catch (const out_of_range &err) { }

  SDL_Rect src = {
    this->x,
    this->y,
    this->w,
    this->h
  };

  SDL_Rect pos = {
    this->parent->x,
    this->parent->y,
    this->w,
    this->h
  };

  SDL_Point emptyPoint = { 0, 0 };

  SDL_RenderCopyEx(
    this->parent->renderer,
    this->parent->texture,
    &src,
    &pos,
    0,
    &emptyPoint,
    this->parent->orientation == Orientation::RIGHT ?
      SDL_FLIP_NONE :
      SDL_FLIP_HORIZONTAL
  );
}

void IdlingState::animate() {
    int total_frames = this->parent->data
      .at("animations")
      .at("idling")
      .at("total_frames").get<int>();
    frame_index = (frame_index + 1) % total_frames;
}

void IdlingState::input(SDL_Event event) {
  switch (event.key.keysym.sym)
	{
		case SDLK_LEFT:
			this->parent->orientation = Orientation::LEFT;
      this->parent->x -= 10;
      break;
		case SDLK_RIGHT:
			this->parent->orientation = Orientation::RIGHT;
      this->parent->x += 10;
      break;
	}
}
