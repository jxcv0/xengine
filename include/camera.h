#ifndef CAMERA_H
#define CAMERA_H

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "math.h"

namespace {
float offsetRad = -5.0f;
glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 _x = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 _y = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 _z = glm::vec3(0.0f, 0.0f, -1.0f);
float _a = 0.0f;
float _b = 90.0f;
float _prevX = 0;
float _prevY = 0;
bool _firstMouseMovement = true;
} // namespace

namespace xen::camera {
// calculate view matrix
glm::mat4 view_matrix() { return glm::lookAt(_position, _position + _z, _y); }

// get camera position
glm::vec3 camera_position() { return _position; }

// init camera to starting values
// TODO condense calls to radians()
void init() {
  _position =
      glm::vec3((offsetRad * (cos(xen::radians(_b)) * cos(xen::radians(_a)))),
                (offsetRad * (sin(xen::radians(_a)))),
                (offsetRad * (sin(xen::radians(_b)) * cos(xen::radians(_a)))));

  _position.y += 3.5f;

  _z = glm::normalize(glm::vec3(cos(xen::radians(_b)) * cos(xen::radians(_a)),
                                sin(xen::radians(_a)),
                                sin(xen::radians(_b)) * cos(xen::radians(_a))));

  _x = glm::normalize(glm::cross(_z, _y));
}

// update _vectors and position based on a centre of rotation about an xz
// position
void update_aim(float mouseX, float mouseY, float sen) {
  if (_firstMouseMovement) {
    _prevX = mouseX;
    _prevY = mouseY;
    _firstMouseMovement = false;
  }

  auto changeX = mouseX - _prevX;
  auto changeY = _prevY - mouseY;

  _prevX = mouseX;
  _prevY = mouseY;

  _b += changeX * sen;
  _a += changeY * sen;

  if (_a > 50.0f) {
    _a = 50.0f;
  };
  if (_a < -50.0f) {
    _a = -50.0f;
  };

  _position =
      glm::vec3((offsetRad * (cos(xen::radians(_b)) * cos(xen::radians(_a)))),
                (offsetRad * (sin(xen::radians(_a)))),
                (offsetRad * (sin(xen::radians(_b)) * cos(xen::radians(_a)))));

  _position.y += 3.5f;

  _z = glm::normalize(glm::vec3(cos(xen::radians(_b)) * cos(xen::radians(_a)),
                                sin(xen::radians(_a)),
                                sin(xen::radians(_b)) * cos(xen::radians(_a))));

  _x = glm::normalize(glm::cross(_z, _y));
}

// update _position based on key press / character movement flags
template <typename Input> void process_movement(Input in, float deltaTime) {
  float velocity = 2.5f * deltaTime;
  if (in.forward()) {
    _position += _z * velocity;
  }
  if (in.backward()) {
    _position -= _z * velocity;
  }
  if (in.left()) {
    _position -= _x * velocity;
  }
  if (in.right()) {
    _position += _x * velocity;
  }
}
} // namespace xen::camera

#endif // CAMERA_H
