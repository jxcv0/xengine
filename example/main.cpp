#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>

#include "checkerr.h"
#include "lin.h"
#include "mat4.h"
#include "shader.h"
#include "vec3.h"
#include "window.h"

constexpr auto window_width = 1080;
constexpr auto window_height = 600;

int main(int argc, char const *argv[]) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  Window window(window_width, window_height, "xengine");

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

  auto projection_matrix = lin::perspective(
      lin::radians(60), ((float)window_width / (float)window_height), 0.1, 100);
  auto view_matrix = lin::translate(Mat4(1), Vec3(0, 0, -3));
  auto model_matrix = lin::rotate(Mat4(1), Vec3(1, 0, 0), lin::radians(-55));

  std::cout << model_matrix << "\n";
  std::cout << glm::to_string(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f),
                                          glm::vec3(1.0f, 0.0f, 0.0f)))
            << "\n\n";

  std::cout << view_matrix << "\n";
  std::cout << glm::to_string(glm::translate(glm::mat4(1.0f),
                                             glm::vec3(0.0f, 0.0f, -3.0f)))
            << "\n\n";

  std::cout << projection_matrix << "\n";
  std::cout << glm::to_string(glm::perspective(
                   glm::radians(60.0f),
                   ((float)window_width / (float)window_height), 0.1f, 100.0f))
            << "\n\n";

  shader.use();
  shader.set_uniform("projection", projection_matrix);
  shader.set_uniform("view", view_matrix);
  shader.set_uniform("model", model_matrix);

  while (!window.should_close()) {
    window.clear_buffers();
    glClearColor(0.2, 0.3, 0.3, 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(0));

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}

// void on_mouse(GLFWwindow *window, double x, double y) {
// xen::camera::update_aim(static_cast<float>(x), static_cast<float>(y), 0.1f);
// }
