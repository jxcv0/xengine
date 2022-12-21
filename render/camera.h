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
class Camera {
 public:
  Camera(Vec3 start_pos, float mouse_start_x, float mouse_start_y)
      : m_pos(start_pos), m_last_mouse_pos(mouse_start_x, mouse_start_y) {}

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
  void process_mouse_movement(const Vec2 *mouse_pos) {
    if (m_first_mouse_movement) {
      m_last_mouse_pos = *mouse_pos;
      m_first_mouse_movement = false;
    }
    Vec2 offset(mouse_pos->x() - m_last_mouse_pos.x(),
                m_last_mouse_pos.y() - mouse_pos->y());
    m_last_mouse_pos = *mouse_pos;

    offset *= m_mouse_sensetivity;

    m_yaw += offset.x();
    m_pitch += offset.y();

    if (m_pitch > 89.0f) {
      m_pitch = 89.0f;
    } else if (m_pitch < -89.0f) {
      m_pitch = -89.0f;
    }

    float yaw = lin::radians(m_yaw);
    float pitch = lin::radians(m_pitch);

    Vec3 temp_view(std::cos(yaw) * std::cos(pitch), std::sin(pitch),
                   std::sin(yaw) * std::cos(pitch));

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
  bool m_first_mouse_movement;
  Vec2 m_last_mouse_pos;
};

#endif  // CAMERA_H_
