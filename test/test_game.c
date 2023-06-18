#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "camera.h"
#include "gamestate.h"
#include "glad.h"
#include "input.h"
#include "lin.h"
#include "pbr_deferred.h"
#include "systems.h"
#include "text.h"
#include "window.h"

#define MAX_ENTITIES 128
#define MAX_GEOMS 64

extern const vec3 GLOBAL_UP;

GLFWwindow *window;
const float window_width = 800;
const float window_height = 600;

struct camera camera;
struct mouse_pos mouse_pos = {0};
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

void update_view_matrix();
void handle_mouse_movement(GLFWwindow *w, double x, double y);
void handle_keyboard_input(GLFWwindow *w);

struct renderer r;

int main() {
  init_mem_subsys();
  camera.m_mouse_sensetivity = 0.3;
  camera.m_movement_speed = 0.15;

  omp_set_num_threads(4);
  omp_set_dynamic(0);

  create_window(&window, window_width, window_height, "game");
  pbrd_init(&r, window_width, window_height);

  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f);

  uint32_t e1;
  create_entity(&e1);
  add_component(e1, MESH_LOAD_REQUEST);
  add_component(e1, MAT_LOAD_REQUEST);
  add_component(e1, MODEL_MATRIX);
  add_component(e1, POSITION);
  add_component(e1, ROTATION);

  union component lr;
  strcpy(lr.as_request.path, "assets/meshes/suzanne.mesh");
  set_component(e1, MESH_LOAD_REQUEST, lr);
  strcpy(lr.as_request.path, "assets/textures/ravine_rock.mtl");
  set_component(e1, MAT_LOAD_REQUEST, lr);

	union component e1_pos = {0};
	union component e1_rot = {0};
	e1_rot.as_rotation.axis[2] = 1.0f;
  set_component(e1, POSITION, e1_pos);
  set_component(e1, ROTATION, e1_rot);

  struct light l = LIGHT_RANGE_3250;
  l.position[0] = 3.0;
  l.position[1] = 3.0;
  l.position[2] = 3.0;
  l.color[0] = 1.0;
  l.color[1] = 1.0;
  l.color[2] = 1.0;

  vec3 camera_centre = {0, 0, 0};

  mouse_pos.m_last_pos[0] = window_width / 2.0f;
  mouse_pos.m_last_pos[1] = window_height / 2.0f;
  camera.m_yaw = 275;

  // TODO
  init_ttf("assets/fonts/Consolas.ttf");

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    get_cursor_position(&mouse_pos, window);
    vec2 cursor_offset;
    get_cursor_offset(cursor_offset, &mouse_pos);
    update_3rd_person_camera(&camera, cursor_offset, 6, camera_centre);
    // handle_keyboard_input(window);
    update_view_matrix();
    float identity[4][4] = IDENTITY_MAT4_INITIALIZER;
    union component matrix;
    memcpy(&matrix, identity, sizeof(identity));

    // TODO update model_matrices
    sys_load(MESH_LOAD_REQUEST);
    sys_load(MAT_LOAD_REQUEST);
		sys_update_model_matrices();
    sys_render_geometries(&r, projection_matrix, view_matrix);
    pbrd_render_lighting(&r, &l, 1, camera.m_pos, window_width, window_height);

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
