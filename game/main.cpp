#include "camera.h"
#include "checkerr.h"
#include "componentarray.h"
#include "constants.h"
#include "entityarray.h"
#include "input.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"
#include "window.h"
#include <pthread.h>
// #include <omp.h>

GLFWwindow *window;
constexpr auto window_width = 1080;
constexpr auto window_height = 600;

struct camera camera;
vec3 mouse_pos;
mat4 view_matrix;

EntityArray entities;
ComponentArray<struct mesh> meshes;

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

  process_mouse_movement(&camera, mouse_pos);
  vec3 temp = {
    camera.m_pos[0] + camera.m_view_dir[0],
    camera.m_pos[1] + camera.m_view_dir[1],
    camera.m_pos[2] + camera.m_view_dir[2]
  };

  look_at(view_matrix, camera.m_pos, temp, camera.m_up);
}

// main
int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  create_window(&window, window_width, window_height, "game");

  shader_t shader = shader_load("render/glsl/uber.vert", "render/glsl/uber.frag");

  mat4 projection_matrix;
  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f); 

  entity_id cube_entity = entities.create();
  meshes.assign(cube_entity);

  meshes.set(cube_entity, mesh_load("assets/models/female_base/female_base.obj"));
  mesh_buffer(meshes.get(cube_entity));

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
    input_poll_cursor_pos(mouse_pos, window);
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

    mesh_draw(meshes.get(cube_entity));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  mesh_unload(meshes.get(cube_entity));

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
