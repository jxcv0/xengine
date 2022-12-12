#include <iostream>

#include "camera.h"
#include "checkerr.h"
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
  auto model_matrix = lin::rotate(Mat4(1), Vec3(1, 0, 0), lin::radians(-55));

  Mesh mesh;
  mesh.load("assets/models/cube/cube.obj");
  mesh.gen_buffers();

  shader.use();
  shader.set_uniform("projection", projection_matrix);
  shader.set_uniform("model", model_matrix);

  while (!window.should_close()) {
    window.clear_buffers();
    glClearColor(0.2, 0.3, 0.3, 1);

    shader.set_uniform("view", camera.view_matrix());

    mesh.draw();

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
