#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"
#include "glad.h"
#include "lin.h"
#include "mesh.h"
#include "render.h"
#include "shader.h"
// #include "text.h"
#include "window.h"

extern const vec3 GLOBAL_UP;

GLFWwindow *window;
float window_width = 1080;
float window_height = 600;

struct camera camera = {.m_mouse_sensetivity = 0.3, .m_movement_speed = 0.1};

vec2 mouse_pos;
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

static void size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  window_width = (float)width;
  window_height = (float)height;
  glViewport(0, 0, width, height);
}

void update_view_matrix() {
  vec3 ctr = {camera.m_pos[0] + camera.m_view_dir[0],
              camera.m_pos[1] + camera.m_view_dir[1],
              camera.m_pos[2] + camera.m_view_dir[2]};

  look_at(view_matrix, camera.m_pos, ctr, camera.m_up);
}

void handle_mouse_movement(GLFWwindow *w, double x, double y) {
  (void)w;
  printf("%f, %f\n", camera.m_pitch, camera.m_yaw);
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

// main
int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  create_editor_window(&window, window_width, window_height, "rig");
  glfwSetFramebufferSizeCallback(window, size_callback);
  // glfwSetCursorPosCallback(window, handle_mouse_movement);

  shader_t mesh_shader =
      load_shader("render/glsl/uber.vert", "render/glsl/uber.frag");

  // shader_t text_shader =
  // shader_load("render/glsl/text.vert", "render/glsl/text.frag");

  struct mesh test_mesh = load_mesh("assets/models/cyborg/cyborg.obj");
  struct mesh floor = load_mesh("assets/models/floor/floor.obj");

  gen_mesh_buffers(&test_mesh);
  gen_mesh_buffers(&floor);

  struct light light = {0};
  light.m_color[0] = 1;
  light.m_color[1] = 1;
  light.m_color[2] = 1;
  light.m_position[0] = 0;
  light.m_position[1] = 2;
  light.m_position[2] = 0;
  light.m_constant = 1.0;
  light.m_linear = 0.35;
  light.m_quadratic = 0.44;

  camera.m_pos[0] = 0;
  camera.m_pos[1] = 1.86;
  camera.m_pos[2] = 3;

  camera.m_last_mouse_pos[0] = window_width / 2.0f;
  camera.m_last_mouse_pos[1] = window_height / 2.0f;
  camera.m_yaw = 275;
  camera.m_pitch = 90;
  update_view_matrix();
  process_mouse_movement(&camera, mouse_pos);
  handle_mouse_movement(window, mouse_pos[0], mouse_pos[1]);

  // init_ttf("assets/fonts/Consolas.ttf");

  // vec4 text_col = {1};

  while (!glfwWindowShouldClose(window)) {
    perspective(projection_matrix, radians(60),
                ((float)window_width / (float)window_height), 0.1f, 100.0f);
    update_view_matrix();
    process_mouse_movement(&camera, mouse_pos);
    handle_keyboard_input(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 1);

    mat4 model;
    identity_mat4(model);

    // TODO this needs to go to struct material as a member of mesh

    draw_mesh(mesh_shader, projection_matrix, view_matrix, model, camera.m_pos,
              &light, &test_mesh);

    draw_mesh(mesh_shader, projection_matrix, view_matrix, model, camera.m_pos,
              &light, &floor);

    // vec2 text_pos = {window_width, window_height};
    // const char *debug_text = "abcdefg";
    // render_text(text_shader, projection_matrix, text_pos, text_col,
    // debug_text, 7);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  unload_mesh(&test_mesh);
  unload_mesh(&floor);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
