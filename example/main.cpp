#include <checkerr.h>
#include <glad.h>
#include <lin.h>
#include <mainwindow.h>
#include <mat4.h>
#include <shader.h>
#include <vec3.h>

#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  auto model_matrix = lin::rotate(Mat4(1), Vec3(1, 0, 0), lin::radians(-55));
  std::cout << model_matrix << "\n";
  std::cout << glm::to_string(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f),
                                          glm::vec3(1.0f, 0.0f, 0.0f)));
  std::cout << "\n";
}

/*
  MainWindow main_window(1080, 600, std::string("main-window"));
  main_window.set_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  main_window.set_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
  main_window.set_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  main_window.set_hint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // main_window.set_cursor_position_callback(on_mouse);
  main_window.show();  // TODO redo main window so that gl context can be
                       // created before calling show.

  auto shader =
      ShaderUtils::load("render/glsl/uber.vert", "render/glsl/uber.frag");

  float vertices[] = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                      -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int vbo, vao, ebo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  auto projection_matrix = main_window.projection_matrix(60);
  auto view_matrix = lin::translate(Mat4(1), Vec3(0, 0, -3));
  auto model_matrix = lin::rotate(Mat4(1), Vec3(1, 0, 0), -55);

  shader.use();
  shader.set_uniform("projection", projection_matrix);
  shader.set_uniform("view", view_matrix);
  shader.set_uniform("model", model_matrix);

  while (!main_window.should_close()) {
    main_window.clear_buffers();
    glClearColor(0.2, 0.3, 0.3, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void *>(0));

    main_window.swap_buffers();
    main_window.poll_events();
  }

  return 0;
}

// void on_mouse(GLFWwindow *window, double x, double y) {
// xen::camera::update_aim(static_cast<float>(x), static_cast<float>(y), 0.1f);
// }
*/
