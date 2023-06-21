#include "camera.h"

const vec3_t GLOBAL_UP = {.elem = {0.0f, 1.0f, 0.0f}};

/**
 * ----------------------------------------------------------------------------
 */
void process_mouse_movement(struct camera *camera, const vec2_t cursor_offset) {
  float offset_x = cursor_offset.x * camera->m_mouse_sensetivity;
  float offset_y = cursor_offset.y * camera->m_mouse_sensetivity;

  camera->m_yaw += offset_x;
  camera->m_pitch += offset_y;

  if (camera->m_pitch > 89.0f) {
    camera->m_pitch = 89.0f;
  } else if (camera->m_pitch < -89.0f) {
    camera->m_pitch = -89.0f;
  }

  float yaw = radians(camera->m_yaw);
  float pitch = radians(camera->m_pitch);

  vec3_t temp_view = {
      .elem = {cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)}};

  normalize_vec3(temp_view);
  camera->m_view_dir = temp_view;

  camera->m_right = normalize_vec3(cross_vec3(camera->m_view_dir, GLOBAL_UP));
  camera->m_up =
      normalize_vec3(cross_vec3(camera->m_right, camera->m_view_dir));
}

/**
 * ----------------------------------------------------------------------------
 */
void update_3rd_person_camera(struct camera *camera, const vec2_t cursor_offset,
                              const float rad, const vec3_t centre) {
  float offset_x = cursor_offset.x * camera->m_mouse_sensetivity;
  float offset_y = cursor_offset.y * camera->m_mouse_sensetivity;

  camera->m_yaw += offset_x;
  camera->m_pitch += offset_y;

  if (camera->m_pitch > 89.0f) {
    camera->m_pitch = 89.0f;
  } else if (camera->m_pitch < -89.0f) {
    camera->m_pitch = -89.0f;
  }

  float yaw = radians(camera->m_yaw);
  float pitch = radians(camera->m_pitch);

  vec3_t temp_view = {
      .elem = {cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)}};

  normalize_vec3(temp_view);
  camera->m_view_dir = temp_view;

  camera->m_right = normalize_vec3(cross_vec3(camera->m_view_dir, GLOBAL_UP));
  camera->m_up =
      normalize_vec3(cross_vec3(camera->m_right, camera->m_view_dir));

  camera->m_pos.x = centre.x + rad * -temp_view.x;
  camera->m_pos.y = centre.y + rad * -temp_view.y;
  camera->m_pos.z = centre.z + rad * -temp_view.z;
}
