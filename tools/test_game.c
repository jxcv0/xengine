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
#include "render.h"
#include "systems.h"
#include "text.h"
#include "window.h"

extern const vec3_t GLOBAL_UP;

const float window_width = 800;
const float window_height = 600;

struct camera camera;
struct mouse_pos mouse_pos = {0};
mat4_t projection_matrix = {0};

mat4_t create_view_matrix();
void handle_mouse_movement(GLFWwindow *w, double x, double y);
void handle_keyboard_input(GLFWwindow *w);

struct renderer r;

int main() {
  init_gamestate();
  camera.mouse_sensetivity = 0.3;
  camera.movement_speed = 0.15;

  omp_set_num_threads(4);
  omp_set_dynamic(0);

  GLFWwindow *window = create_window(window_width, window_height, "game");
  //  assert(init_text("assets/fonts/Consolas.ttf") != -1);
  init_renderer(&r, window_width, window_height);

  projection_matrix =
      perspective(radians(60.0f), ((float)window_width / (float)window_height),
                  0.1f, 100.0f);

  uint32_t e1;
  create_obj(&e1);
  add_attrib(e1, attrib_type_MESH_LOAD_REQUEST);
  add_attrib(e1, attrib_type_MAT_LOAD_REQUEST);
  add_attrib(e1, attrib_type_MODEL_MATRIX);
  add_attrib(e1, attrib_type_POSITION);
  add_attrib(e1, attrib_type_ROTATION);

  attrib_t lr;
  strcpy(lr.as_request.path, "assets/meshes/suzanne.mesh");
  set_attrib(e1, attrib_type_MESH_LOAD_REQUEST, lr);
  strcpy(lr.as_request.path, "assets/textures/ravine_rock.mtl");
  set_attrib(e1, attrib_type_MAT_LOAD_REQUEST, lr);

  attrib_t e1_pos = {0};
  attrib_t e1_rot = {0};
  attrib_t e1_mm = {0};
  e1_mm.as_model_matrix = identitym4();
  e1_rot.as_rotation.axis.x = 1.0f;
  set_attrib(e1, attrib_type_POSITION, e1_pos);
  set_attrib(e1, attrib_type_ROTATION, e1_rot);
  set_attrib(e1, attrib_type_MODEL_MATRIX, e1_mm);

  struct light l = LIGHT_RANGE_3250;
  l.position.x = 3.0;
  l.position.y = 3.0;
  l.position.z = 3.0;
  l.color.x = 1.0;
  l.color.y = 1.0;
  l.color.z = 1.0;

  vec3_t camera_centre = {0};

  mouse_pos.m_last_pos.x = window_width / 2.0f;
  mouse_pos.m_last_pos.y = window_height / 2.0f;
  camera.yaw = 275.0f;

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    union attribute rot = get_attrib(e1, attrib_type_ROTATION);
    rot.as_rotation.radians += radians(0.01f);
    set_attrib(e1, attrib_type_ROTATION, rot);

    get_cursor_position(&mouse_pos, window);
    vec2_t cursor_offset = get_cursor_offset(&mouse_pos);
    update_3rd_person_camera(&camera, cursor_offset, 3, camera_centre);
    // handle_keyboard_input(window);

    sys_load(attrib_type_MESH_LOAD_REQUEST);
    sys_load(attrib_type_MAT_LOAD_REQUEST);
    sys_update_model_matrices();
    sys_render_geometries(&r, projection_matrix, create_view_matrix());
    render_lighting(&r, &l, 1, camera.pos.elem, window_width, window_height);

    // TODO Rendering to wrong buffer?
    // float text_col[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // render_text(25.0f, 15.0f, 10.0f, "Sample Text", text_col);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

mat4_t create_view_matrix() {
  vec3_t ctr = {.elem = {camera.pos.x + camera.view_dir.x,
                         camera.pos.y + camera.view_dir.y,
                         camera.pos.z + camera.view_dir.z}};

  return look_at(camera.pos, ctr, camera.up);
}

// move the fps camera with wasd and update mouse input
void handle_keyboard_input(GLFWwindow *w) {
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    camera.pos.x += camera.view_dir.x * camera.movement_speed;
    camera.pos.y += camera.view_dir.y * camera.movement_speed;
    camera.pos.z += camera.view_dir.z * camera.movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    camera.pos.x -= camera.view_dir.x * camera.movement_speed;
    camera.pos.y -= camera.view_dir.y * camera.movement_speed;
    camera.pos.z -= camera.view_dir.z * camera.movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    camera.pos.x -= camera.right.x * camera.movement_speed;
    camera.pos.y -= camera.right.y * camera.movement_speed;
    camera.pos.z -= camera.right.z * camera.movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    camera.pos.x += camera.right.x * camera.movement_speed;
    camera.pos.y += camera.right.y * camera.movement_speed;
    camera.pos.z += camera.right.z * camera.movement_speed;
  }
}
