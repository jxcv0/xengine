#ifndef CAMERA_H_
#define CAMERA_H_

#include "lin.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const vec3 GLOBAL_UP;

/**
 * @brief Camera class for calculating view matrix.
 */
struct camera {
  float m_yaw;
  float m_pitch;
  float m_movement_speed;
  float m_mouse_sensetivity;
  vec3 m_pos;
  vec3 m_view_dir;
  vec3 m_up;
  vec3 m_right;
  vec2 m_last_mouse_pos;
  bool m_first_mouse_movement;
};

/**
 * @brief Update camera state based on mouse movement.
 *
 * @param camera The camera to update
 * @param x_offset Mouse movement offset in x.
 * @param y_offset Mouse movement offset in y.
 */
void process_mouse_movement(struct camera *camera, const vec2 mouse_pos);

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_H_
