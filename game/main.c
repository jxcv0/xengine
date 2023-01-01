#include <glad.h>
#include <GLFW/glfw3.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "camera.h"
#include "input.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"
#include "window.h"

extern const vec3 GLOBAL_UP;

void print_mat4(const char *name, const mat4 m) {
  printf("mat4: %s {\n", name);
  for (int i = 0; i < 4; i++) {
    printf("%f, %f, %f, %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }
  printf("}\n");
}

GLFWwindow *window;
const float window_width = 1080;
const float window_height = 600;

struct camera camera = {.m_mouse_sensetivity = 0.3, .m_movement_speed = 0.5};

vec3 mouse_pos;
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

// move the fps camera with wasd and update mouse input
void handle_player_input() {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] += forward[0] * 0.2f;
    camera.m_pos[1] += forward[1] * 0.2f;
    camera.m_pos[2] += forward[2] * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] -= forward[0] * 0.2f;
    camera.m_pos[1] -= forward[1] * 0.2f;
    camera.m_pos[2] -= forward[2] * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.m_pos[0] -= camera.m_right[0] * 0.2f;
    camera.m_pos[1] -= camera.m_right[1] * 0.2f;
    camera.m_pos[2] -= camera.m_right[2] * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.m_pos[0] += camera.m_right[0] * 0.2f;
    camera.m_pos[1] += camera.m_right[1] * 0.2f;
    camera.m_pos[2] += camera.m_right[2] * 0.2f;
  }

  vec3 ctr = {camera.m_pos[0] + camera.m_view_dir[0],
              camera.m_pos[1] + camera.m_view_dir[1],
              camera.m_pos[2] + camera.m_view_dir[2]};

  look_at(view_matrix, camera.m_pos, ctr, camera.m_up);
}

// main
int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  create_window(&window, window_width, window_height, "game");

  shader_t shader =
      shader_load("render/glsl/uber.vert", "render/glsl/uber.frag");

  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f);

  struct mesh cube_mesh =
      mesh_load("assets/models/female_base/female_base.obj");
  mesh_buffer(&cube_mesh);

  glUseProgram(shader);
  shader_set_uniform_m4fv(shader, "projection", projection_matrix);

  vec3 light_pos = {2, 3, 2};
  vec3 light_color = {1, 1, 1};
  vec3 object_color = {1, 0.5, 0.31};

  camera.m_pos[0] = 0;
  camera.m_pos[1] = 1.86;
  camera.m_pos[2] = 3;

  camera.m_last_mouse_pos[0] = window_width / 2.0f;
  camera.m_last_mouse_pos[1] = window_height / 2.0f;

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    input_poll_cursor_pos(mouse_pos, window);
    process_mouse_movement(&camera, mouse_pos);
    handle_player_input();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 1);

    mat4 model;
    identity_mat4(model);
    // transforms
    shader_set_uniform_m4fv(shader, "model", model);
    shader_set_uniform_m4fv(shader, "view", view_matrix);

    // lighting
    shader_set_uniform_3fv(shader, "light_pos", light_pos);
    shader_set_uniform_3fv(shader, "light_color", light_color);
    shader_set_uniform_3fv(shader, "obj_color", object_color);

    mesh_draw(&cube_mesh);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  mesh_unload(&cube_mesh);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}