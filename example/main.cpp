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

  float vertices[] = {
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int vbo, vao, ebo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

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
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(0));

    gl_print_error(std::cout, __FILE__, __LINE__);
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
