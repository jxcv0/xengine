#include "camera.h"

const vec3_t GLOBAL_UP = {.elem = {0.0f, 1.0f, 0.0f}};

/**
 * ----------------------------------------------------------------------------
 */
void process_mouse_movement(struct camera *camera, const vec2_t cursor_offset) {
  float offset_x = cursor_offset.x * camera->mouse_sensetivity;
  float offset_y = cursor_offset.y * camera->mouse_sensetivity;

  camera->yaw += offset_x;
  camera->pitch += offset_y;

  if (camera->pitch > 89.0f) {
    camera->pitch = 89.0f;
  } else if (camera->pitch < -89.0f) {
    camera->pitch = -89.0f;
  }

  float yaw = radians(camera->yaw);
  float pitch = radians(camera->pitch);

  vec3_t temp_view = {
      .elem = {cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)}};

  normalizev3(temp_view);
  camera->view_dir = temp_view;

  camera->right = normalizev3(crossv3(camera->view_dir, GLOBAL_UP));
  camera->up = normalizev3(crossv3(camera->right, camera->view_dir));
}

/**
 * ----------------------------------------------------------------------------
 */
void update_3rd_person_camera(struct camera *camera, const vec2_t cursor_offset,
                              const float rad, const vec3_t centre) {
  float offset_x = cursor_offset.x * camera->mouse_sensetivity;
  float offset_y = cursor_offset.y * camera->mouse_sensetivity;

  camera->yaw += offset_x;
  camera->pitch += offset_y;

  if (camera->pitch > 89.0f) {
    camera->pitch = 89.0f;
  } else if (camera->pitch < -89.0f) {
    camera->pitch = -89.0f;
  }

  float yaw = radians(camera->yaw);
  float pitch = radians(camera->pitch);

  vec3_t temp_view = {
      .elem = {cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)}};

  normalizev3(temp_view);
  camera->view_dir = temp_view;

  camera->right = normalizev3(crossv3(camera->view_dir, GLOBAL_UP));
  camera->up = normalizev3(crossv3(camera->right, camera->view_dir));

  camera->pos.x = centre.x + rad * -temp_view.x;
  camera->pos.y = centre.y + rad * -temp_view.y;
  camera->pos.z = centre.z + rad * -temp_view.z;
}
