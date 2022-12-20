#include <math.h>

#include <iostream>

#include "camera.h"
#include "checkerr.h"
#include "componentarray.h"
#include "entityarray.h"
#include "lin.h"
#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "vec3.h"
#include "window.h"

void on_mouse(GLFWwindow *window, double x, double y);

constexpr auto window_width = 1080;
constexpr auto window_height = 600;
float last_x = window_width / 2.0f;
float last_y = window_height / 2.0f;
bool first_mouse = true;

Camera camera;

EntityArray entities;
ComponentArray<Mesh> mesh_components;
ComponentArray<Vec3> translation_components;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  Window window(window_width, window_height, "xengine");
  window.set_cursor_position_callback(on_mouse);

  auto shader =
      ShaderUtils::load("render/glsl/uber.vert", "render/glsl/uber.frag");

  auto projection_matrix = window.projection_matrix(60);

  // entity
  entity_id id = entities.create();
  mesh_components.assign(id);
  translation_components.assign(id);

  translation_components.set(id, Vec3(0, 0, 0));

  Mat4 model_matrix = lin::translate(Mat4(1), *translation_components.get(id));
  model_matrix = lin::rotate(model_matrix, Vec3(1, 0.8, 0), lin::radians(-55));

  mesh_components.get(id)->load("assets/models/cube/cube.obj");
  mesh_components.get(id)->gen_buffers();

  shader.use();
  shader.set_uniform("projection", &projection_matrix);

  Vec3 light_pos(1, 1, 1);
  Vec3 light_color(1, 1, 1);
  Vec3 object_color(1, 0.5, 0.31);

  while (!window.should_close()) {
    window.clear_buffers();
    glClearColor(0.2, 0.3, 0.3, 1);

    // transforms
    shader.set_uniform("model", &model_matrix);
    Mat4 view_matrix = camera.view_matrix();
    shader.set_uniform("view", &view_matrix);

    // lighting
    shader.set_uniform("light_pos", &light_pos);
    shader.set_uniform("light_color", &light_color);
    shader.set_uniform("obj_color", &object_color);

    mesh_components.get(id)->draw();

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}

void on_mouse([[maybe_unused]] GLFWwindow *window, double x, double y) {
  auto x_pos = static_cast<float>(x);
  auto y_pos = static_cast<float>(y);
  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }
  float x_offset = x_pos - last_x;
  float y_offset = last_y - y_pos;
  last_x = x_pos;
  last_y = y_pos;
  camera.process_mouse_movement(x_offset, y_offset);
}
