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
 * @param cursor_offset The change in the cursor position.
 */
void process_mouse_movement(struct camera *camera, const vec2 cursor_offset);

/**
 * @brief Update a camera as a 3rd person camera based on change in cursor
 * position.
 *
 * @param camera The camera to store the data in.
 * @param cursor_offset The change in the cursor postition.
 * @param rad The distance of the camera from centre.
 * @param centre The centre to rotate the camera around.
 */
void update_3rd_person_camera(struct camera *camera, const vec2 cursor_offset,
                              const float rad, const vec3 centre);

#ifdef __cplusplus
}
#endif

#endif  // CAMERA_H_
