#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camera.h"
#include "glad.h"
#include "input.h"
#include "lin.h"
#include "mesh.h"
#include "render.h"
#include "window.h"

#define MAX_MESHES 64

extern const vec3 GLOBAL_UP;

GLFWwindow *window;
const float window_width = 1080;
const float window_height = 600;

struct camera camera = {.m_mouse_sensetivity = 0.3, .m_movement_speed = 0.1};
vec2 mouse_pos;
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

void update_view_matrix();
void handle_mouse_movement(GLFWwindow *w, double x, double y);
void handle_keyboard_input(GLFWwindow *w);

// main
int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  create_window(&window, window_width, window_height, "game");
  glfwSetCursorPosCallback(window, handle_mouse_movement);

  struct renderer r;
  if (renderer_init(&r, window_width, window_height) != 0) {
    fprintf(stderr, "Unable to initialize renderer. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f);

  struct mesh test_mesh = load_mesh("cyborg.model");

  struct light light = {0};
  light.m_color[0] = 1;
  light.m_color[1] = 1;
  light.m_color[2] = 1;
  light.m_position[0] = 0;
  light.m_position[1] = 2;
  light.m_position[2] = 3;
  light.m_constant = 1.0;
  light.m_linear = 0.07;
  light.m_quadratic = 0.18;

  camera.m_pos[0] = 0;
  camera.m_pos[1] = 1.86;
  camera.m_pos[2] = 3;

  camera.m_last_mouse_pos[1] = window_height / 2.0f;
  camera.m_yaw = 275;
  process_mouse_movement(&camera, mouse_pos);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    handle_keyboard_input(window);
    update_view_matrix();

    mat4 model_matrix;
    identity_mat4(model_matrix);

    render_geometries(&r, projection_matrix, view_matrix, &model_matrix,
                      &test_mesh, 1);
    render_lighting(&r, &light, 1, camera.m_pos);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  unload_mesh(&test_mesh);

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

void handle_mouse_movement(GLFWwindow *w, double x, double y) {
  (void)w;
  mouse_pos[0] = (float)x;
  mouse_pos[1] = (float)y;
  process_mouse_movement(&camera, mouse_pos);
}

// move the fps camera with wasd and update mouse input
void handle_keyboard_input(GLFWwindow *w) {
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] += forward[0] * camera.m_movement_speed;
    camera.m_pos[1] += forward[1] * camera.m_movement_speed;
    camera.m_pos[2] += forward[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] -= forward[0] * camera.m_movement_speed;
    camera.m_pos[1] -= forward[1] * camera.m_movement_speed;
    camera.m_pos[2] -= forward[2] * camera.m_movement_speed;
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
