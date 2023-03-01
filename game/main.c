#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "camera.h"
#include "glad.h"
#include "input.h"
#include "lin.h"
#include "pbr_deferred.h"
#include "window.h"

#define MAX_ENTITIES 128
#define MAX_GEOMS 64

extern const vec3 GLOBAL_UP;

GLFWwindow *window;
const float window_width = 800;
const float window_height = 600;

// struct light light_array[MAX_NUM_LIGHTS] = {0};
vec3 positions[MAX_ENTITIES] = {0};
mat4 model_matrices[MAX_ENTITIES] = {0};

struct camera camera = {.m_mouse_sensetivity = 0.3, .m_movement_speed = 0.15};
struct mouse_pos mouse_pos = {0};
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

uint32_t num_geoms = 0;
struct geometry geometry_arr[MAX_GEOMS] = {0};

struct geometry *alloc_geom(size_t n) {
  assert(num_geoms < MAX_GEOMS);
  struct geometry *temp = &geometry_arr[num_geoms];
  num_geoms += n;
  return temp;
}

struct pbr_material *alloc_mat(size_t n) {
  return calloc(n, sizeof(struct pbr_material));
};

void update_view_matrix();
void handle_mouse_movement(GLFWwindow *w, double x, double y);
void handle_keyboard_input(GLFWwindow *w);

// main
int main() {
  omp_set_num_threads(4);
  omp_set_dynamic(0);

  create_window(&window, window_width, window_height, "game");
  pbrd_init(window_width, window_height);

  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f);

  load_model(
      "assets/models/Superheroine_Basemesh_No_Hair/"
      "Superheroine_Basemesh_No_Hair.model",
      alloc_geom);

  struct pbr_material material = load_pbr_material("ravine_rock");
  struct geometry pbr_sphere = load_geometry("assets/meshes/Sphere-mesh.geom");

  struct light l = LIGHT_RANGE_3250;
  l.m_position[0] = 3.0;
  l.m_position[1] = 3.0;
  l.m_position[2] = 0.0;
  l.m_color[0] = 1.0;
  l.m_color[1] = 1.0;
  l.m_color[2] = 1.0;

  vec3 camera_centre = {0};

  mouse_pos.m_last_pos[0] = window_width / 2.0f;
  mouse_pos.m_last_pos[1] = window_height / 2.0f;
  camera.m_yaw = 275;

  mat4 identity = IDENTITY_MAT4;
  mat4 model_matrix = {0};
  vec3 rot = {1, 0, 0};
  vec3 pos = {0, 0, 0};
  rotate(model_matrix, identity, rot, radians(-90));
  translate(model_matrix, pos);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    get_cursor_position(&mouse_pos, window);
    vec2 cursor_offset;
    get_cursor_offset(cursor_offset, &mouse_pos);
    update_3rd_person_camera(&camera, cursor_offset, 2, camera_centre);
    // handle_keyboard_input(window);
    update_view_matrix();

    // TODO material does not match geometry.
    pbrd_render_geometries(projection_matrix, view_matrix, &model_matrix,
                           geometry_arr, num_geoms);
    pbrd_render_lighting(&l, 1, camera.m_pos, window_width, window_height);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void update_view_matrix() {
  vec3 ctr = {camera.m_pos[0] + camera.m_view_dir[0],
              camera.m_pos[1] + camera.m_view_dir[1],
              camera.m_pos[2] + camera.m_view_dir[2]};

  look_at(view_matrix, camera.m_pos, ctr, camera.m_up);
}

// move the fps camera with wasd and update mouse input
void handle_keyboard_input(GLFWwindow *w) {
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    camera.m_pos[0] += camera.m_view_dir[0] * camera.m_movement_speed;
    camera.m_pos[1] += camera.m_view_dir[1] * camera.m_movement_speed;
    camera.m_pos[2] += camera.m_view_dir[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    camera.m_pos[0] -= camera.m_view_dir[0] * camera.m_movement_speed;
    camera.m_pos[1] -= camera.m_view_dir[1] * camera.m_movement_speed;
    camera.m_pos[2] -= camera.m_view_dir[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    camera.m_pos[0] -= camera.m_right[0] * camera.m_movement_speed;
    camera.m_pos[1] -= camera.m_right[1] * camera.m_movement_speed;
    camera.m_pos[2] -= camera.m_right[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    camera.m_pos[0] += camera.m_right[0] * camera.m_movement_speed;
    camera.m_pos[1] += camera.m_right[1] * camera.m_movement_speed;
    camera.m_pos[2] += camera.m_right[2] * camera.m_movement_speed;
  }
}
