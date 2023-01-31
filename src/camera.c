#include "camera.h"

const vec3 GLOBAL_UP = {0, 1, 0};

/**
 * ----------------------------------------------------------------------------
 */
void process_mouse_movement(struct camera *camera, const vec2 cursor_offset) {
  float offset_x = cursor_offset[0] * camera->m_mouse_sensetivity;
  float offset_y = cursor_offset[1] * camera->m_mouse_sensetivity;

  camera->m_yaw += offset_x;
  camera->m_pitch += offset_y;

  if (camera->m_pitch > 89.0f) {
    camera->m_pitch = 89.0f;
  } else if (camera->m_pitch < -89.0f) {
    camera->m_pitch = -89.0f;
  }

  float yaw = radians(camera->m_yaw);
  float pitch = radians(camera->m_pitch);

  vec3 temp_view = {cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)};

  normalize_vec3(temp_view);
  camera->m_view_dir[0] = temp_view[0];
  camera->m_view_dir[1] = temp_view[1];
  camera->m_view_dir[2] = temp_view[2];

  cross_vec3(camera->m_right, camera->m_view_dir, GLOBAL_UP);
  cross_vec3(camera->m_up, camera->m_right, camera->m_view_dir);
  normalize_vec3(camera->m_right);
  normalize_vec3(camera->m_up);
}
