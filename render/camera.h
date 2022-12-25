#ifndef CAMERA_H_
#define CAMERA_H_

#include "lin.h"
#include "mat4.h"
#include "vec2.h"
#include "vec3.h"

constexpr Vec3 GLOBAL_UP(0, 1, 0);

/**
 * @brief Camera class for calculating view matrix.
 */
struct Camera {
  float m_yaw = 90;
  float m_pitch = 0;
  // float m_movement_speed = 1;
  float m_mouse_sensetivity = 1.0f;
  Vec3 m_pos = Vec3(0, 0, 0);
  Vec3 m_view_dir;
  Vec3 m_up;
  Vec3 m_right;
  bool m_first_mouse_movement;
  Vec2 m_last_mouse_pos;
};

/**
 * @brief Update camera state based on mouse movement.
 *
 * @param camera The camera to update
 * @param x_offset Mouse movement offset in x.
 * @param y_offset Mouse movement offset in y.
 */
void process_mouse_movement(Camera *camera, const Vec2 *mouse_pos) {
  if (camera->m_first_mouse_movement) {
    camera->m_last_mouse_pos = *mouse_pos;
    camera->m_first_mouse_movement = false;
  }
  Vec2 offset(mouse_pos->x() - camera->m_last_mouse_pos.x(),
              camera->m_last_mouse_pos.y() - mouse_pos->y());
  camera->m_last_mouse_pos = *mouse_pos;

  offset *= camera->m_mouse_sensetivity;

  camera->m_yaw += offset.x();
  camera->m_pitch += offset.y();

  if (camera->m_pitch > 89.0f) {
    camera->m_pitch = 89.0f;
  } else if (camera->m_pitch < -89.0f) {
    camera->m_pitch = -89.0f;
  }

  float yaw = lin::radians(camera->m_yaw);
  float pitch = lin::radians(camera->m_pitch);

  Vec3 temp_view(std::cos(yaw) * std::cos(pitch), std::sin(pitch),
                 std::sin(yaw) * std::cos(pitch));

  camera->m_view_dir = temp_view.normalize();
  camera->m_right = (camera->m_view_dir * GLOBAL_UP).normalize();
  camera->m_up = (camera->m_right * camera->m_view_dir).normalize();
}

#endif  // CAMERA_H_
