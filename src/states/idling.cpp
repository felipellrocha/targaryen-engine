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

BaseState* IdlingState::update() {
  //this->parent->y += 1;

  return NULL;
}

BaseState* IdlingState::input(SDL_Event event) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym)
    {
      case SDLK_UP:
        return new JumpingState(this->parent);
      case SDLK_LEFT:
        this->parent->orientation = Orientation::LEFT;
        return new RunningState(this->parent);
      case SDLK_RIGHT:
        this->parent->orientation = Orientation::RIGHT;
        return new RunningState(this->parent);
    }
	}

  return NULL;
}
