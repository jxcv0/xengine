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
#include <cassert>

constexpr auto window_width = 1080;
constexpr auto window_height = 600;
GLFWwindow *window;

Camera camera(Vec3(0, 0, 3), window_width / 2.0f, window_height / 2.0f);

EntityArray entities;
ComponentArray<Mesh> meshes;
ComponentArray<Mat4> transformations;
ComponentArray<input_handler_fp> input_handlers;  // input handler functions

void load_meshes() {
  for (int i = 0; i < meshes.size(); i++) {
    // load
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  create_window(&window, window_width, window_height, "game");
  Input input(window);

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

  while (!glfwWindowShouldClose(window)) {
    mouse_pos = input.poll_cursor_pos();
    camera.process_mouse_movement(&mouse_pos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 1);

    // transforms
    Mat4 view_matrix = camera.view_matrix();
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
