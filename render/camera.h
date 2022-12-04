#ifndef CAMERA_H_
#define CAMERA_H_

#include "lin.h"
#include "mat4.h"
#include "vec3.h"

constexpr Vec3 GLOBAL_UP(0, 1, 0);

/**
 * @brief Camera class for calculating view matrix.
 */
class Camera {
 public:
  Camera() : m_pos(0, 0, 3) {}

  /**
   * @brief Calculate a view matrix based on camera position.
   *
   * @return a Mat4 view matrix.
   */
  auto view_matrix() { return lin::look_at(m_pos, m_pos + m_view_dir, m_up); }

  /**
   * @brief Update camera state based on mouse movement.
   *
   * @param x_offset Mouse movement offset in x.
   * @param y_offset Mouse movement offset in y.
   */
  void process_mouse_movement(float x_offset, float y_offset) {
    x_offset *= m_mouse_sensetivity;
    y_offset *= m_mouse_sensetivity;
    m_yaw += x_offset;
    m_pitch += y_offset;

    if (m_pitch > 89.0f) {
      m_pitch = 89.0f;
    } else if (m_pitch < -89.0f) {
      m_pitch = -89.0f;
    }

    Vec3 temp_view(
        std::cos(lin::radians(m_yaw)) * std::cos(lin::radians(m_pitch)),
        std::sin(lin::radians(m_pitch)),
        std::sin(lin::radians(m_yaw)) * std::cos(lin::radians(m_pitch)));

    m_view_dir = temp_view.normalize();
    m_right = (m_view_dir * GLOBAL_UP).normalize();
    m_up = (m_right * m_view_dir).normalize();
  }

 private:
  float m_yaw = 0;
  float m_pitch = 0;
  // float m_movement_speed = 1;
  float m_mouse_sensetivity = 1.0f;
  Vec3 m_pos;
  Vec3 m_view_dir;
  Vec3 m_up;
  Vec3 m_right;
};

#endif  // CAMERA_H_
