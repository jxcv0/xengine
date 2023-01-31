#ifndef CAMERA_H_
#define CAMERA_H_

#include "lin.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

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
};

/**
 * @brief Update camera state based on change in cursor position.
 *
 * @param camera Pointer to the camera to update.
 * @param cursor_offset The change in the camera position.
 */
void process_mouse_movement(struct camera *camera, const vec2 cursor_offset);

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_H_
