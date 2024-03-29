#ifndef CAMERA_H_
#define CAMERA_H_

#include "attrib.h"
#include "lin.h"

/**
 * @brief data for calculating view matrix.
 */
struct camera {
  float yaw;
  float pitch;
  float movement_speed;
  float mouse_sensetivity;
  vec3_t pos;
  vec3_t view_dir;
  vec3_t up;
  vec3_t right;
};

/**
 * @brief Update camera state based on change in cursor position.
 *
 * @param camera Pointer to the camera to update.
 * @param cursor_offset The change in the cursor position.
 */
void process_mouse_movement(struct camera *camera, const vec2_t cursor_offset);

/**
 * @brief Update a camera as a 3rd person camera based on change in cursor
 * position.
 *
 * @param camera The camera to store the data in.
 * @param cursor_offset The change in the cursor postition.
 * @param rad The distance of the camera from centre.
 * @param centre The centre to rotate the camera around.
 */
void update_3rd_person_camera(struct camera *camera, const vec2_t cursor_offset,
                              const float rad, const vec3_t centre);

#endif  // CAMERA_H_
