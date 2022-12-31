#include "camera.h"

void process_mouse_movement(struct camera *camera, const vec2 mouse_pos) {
  if (camera->m_first_mouse_movement) {
    camera->m_last_mouse_pos[0] = mouse_pos[0];
    camera->m_last_mouse_pos[1] = mouse_pos[1];
    camera->m_first_mouse_movement = false;
  }

  vec2 offset = {
      mouse_pos[0] - camera->m_last_mouse_pos[0],
      camera->m_last_mouse_pos[1]- mouse_pos[1]
  };

  camera->m_last_mouse_pos[0] = mouse_pos[0];
  camera->m_last_mouse_pos[1] = mouse_pos[1];

  offset[0] *= camera->m_mouse_sensetivity;
  offset[1] *= camera->m_mouse_sensetivity;

  camera->m_yaw += offset[0];
  camera->m_pitch += offset[1];

  if (camera->m_pitch > 89.0f) {
    camera->m_pitch = 89.0f;
  } else if (camera->m_pitch < -89.0f) {
    camera->m_pitch = -89.0f;
  }

  float yaw = radians(camera->m_yaw);
  float pitch = radians(camera->m_pitch);

  vec3 temp_view = {
      cos(yaw) * cos(pitch),
      sin(pitch),
      sin(yaw) * cos(pitch)
  };

  normalize_vec3(temp_view);
  camera->m_view_dir[0] = temp_view[0];
  camera->m_view_dir[1] = temp_view[1];
  camera->m_view_dir[2] = temp_view[2];

  cross_vec3(camera->m_right, camera->m_view_dir, GLOBAL_UP);
  cross_vec3(camera->m_up, camera->m_right, GLOBAL_UP);
  normalize_vec3(camera->m_right);
  normalize_vec3(camera->m_up);
}
