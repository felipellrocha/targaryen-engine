#include "running.h"

RunningState::RunningState(CharacterLayer * parent) {
  this->parent = parent;
}

void RunningState::render() {
  try {
    // that to_string just *cannot* be efficient... Alas...
    auto frame = this->parent->data
      .at("animations")
      .at("running")
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

void RunningState::moveAnimationFrame() {
  int total_frames = this->parent->data
    .at("animations")
    .at("running")
    .at("total_frames").get<int>();
  frame_index = (frame_index + 1) % total_frames;
}

BaseState* RunningState::update() {
  this->moveAnimationFrame();

  switch (this->parent->orientation)
  {
    case Orientation::LEFT:
      this->parent->x -= 2;
      break;
    case Orientation::RIGHT:
      this->parent->x += 2;
      break;
  }

  // uint is necessary here simply to disambiguate
  // the constructor call
  this->parent->aabb = AABB(
    (uint)this->parent->x,
    (uint)this->parent->y,
    0,
    this->parent->x + this->w,
    this->parent->y + this->h,
    0
  );

  return NULL;
}

BaseState* RunningState::input(SDL_Event event) {
  if (event.type == SDL_KEYUP) {
    switch (event.key.keysym.sym)
    {
      case SDLK_LEFT:
        return new IdlingState(this->parent);
      case SDLK_RIGHT:
        return new IdlingState(this->parent);
    }
  }

  return NULL;
}
