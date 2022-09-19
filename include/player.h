#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "scene.h"

namespace {
unsigned int _model;
}  // namespace

namespace xen::player {
// set the player model
void set_model(int m) { _model = m; }

// process keyboard input
void process_input() {
  if (input::forward()) {
    xen::scene::set_model_rotation_y(_model, 0.0f);
    return;
  } else if (input::backward()) {
    xen::scene::set_model_rotation_y(_model, 180.0f);
    return;
  }

  if (input::left()) {
  xen:
    scene::set_model_rotation_y(_model, 90.0f);
    return;
  } else if (input::right()) {
    xen::scene::set_model_rotation_y(_model, 270.0f);
    return;
  }
}
}  // namespace xen::player

#endif  // PLAYER_H
