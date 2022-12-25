#include <GLFW/glfw3.h>

#include "camera.h"
#include "checkerr.h"
#include "componentarray.h"
#include "entityarray.h"
#include "input.h"
#include "lin.h"
#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "threadpool.h"
#include "vec2.h"
#include "vec3.h"
#include "window.h"

GLFWwindow *window;
constexpr auto window_width = 1080;
constexpr auto window_height = 600;

Camera camera;

EntityArray entities;
ComponentArray<Mesh> meshes;
ComponentArray<Mat4> transformations;
ComponentArray<input_handler_fp> input_handlers;  // input handler functions

void load_meshes() {
  for (int i = 0; i < meshes.size(); i++) {
    // load
  }
}

// move the camera with wasd
void handle_player_input() {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.m_pos -= camera.m_view_dir * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.m_pos += camera.m_view_dir * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.m_pos -= camera.m_right * 0.2f;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.m_pos += camera.m_right * 0.2f;
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  create_window(&window, window_width, window_height, "game");

  auto shader =
      ShaderUtils::load("render/glsl/uber.vert", "render/glsl/uber.frag");

  auto projection_matrix = lin::perspective(
      lin::radians(60), ((float)window_width / (float)window_height), 0.1f,
      100.0f);

  // entity
  entity_id id = entities.create();
  meshes.assign(id);
  transformations.assign(id);

  meshes.set(id, load_mesh("assets/models/cube/cube.obj"));
  gen_mesh_buffers(meshes.get(id));

  shader.use();
  shader.set_uniform("projection", &projection_matrix);

  Vec3 light_pos(1, 1, 1);
  Vec3 light_color(1, 1, 1);
  Vec3 object_color(1, 0.5, 0.31);
  Vec2 mouse_pos;

  camera.m_pos = Vec3(0, 0, 3);
  camera.m_last_mouse_pos = Vec2(window_width / 2.0f, window_height / 2.0f);

  while (!glfwWindowShouldClose(window)) {
    mouse_pos = poll_cursor_pos(window);
    process_mouse_movement(&camera, &mouse_pos);
    handle_player_input();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 1);

    // transforms
    Mat4 view_matrix = lin::look_at(
        camera.m_pos, camera.m_pos + camera.m_view_dir, camera.m_up);

    shader.set_uniform("model", transformations.get(id));
    shader.set_uniform("view", &view_matrix);

    // lighting
    shader.set_uniform("light_pos", &light_pos);
    shader.set_uniform("light_color", &light_color);
    shader.set_uniform("obj_color", &object_color);

    // mesh_components.get(id)->draw();
    draw_mesh(meshes.get(id));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
