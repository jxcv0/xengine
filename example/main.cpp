#include <glad.h>
#include <mainwindow.h>
#include <shader.h>

#include "lin.h"
#include "vec3.h"
// #include <mesh.h>

#include <iostream>

// TODO copy learnopengl code for cube here then replace with Resource<Mesh>

int main(int argc, char const *argv[]) {
  MainWindow main_window(1080, 600, std::string("main-window"));
  main_window.set_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  main_window.set_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
  main_window.set_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // main_window.set_cursor_position_callback(on_mouse);
  main_window.show();  // TODO redo main window

  auto shader =
      ShaderUtils::load("render/glsl/uber.vert", "render/glsl/uber.frag");

  unsigned int vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  float vertices[] = {
    // TODO make cube
  };

  // TODO set up buffers for cube
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  auto projection_matrix = main_window.projection_matrix(60.0f);
  Mat4 view_matrix(1.0f);
  view_matrix = lin::translate(view_matrix, Vec3(0.0f, 0.0f, -3.0f));
  Mat4 model_matrix(1.0f);
  model_matrix = lin::rotate(model_matrix, Vec3(1.0f, 0.0f, 0.0f), -55.0f);

  shader.set_uniform("projection", projection_matrix);
  shader.set_uniform("view", view_matrix);
  shader.set_uniform("model", model_matrix);
  shader.use();

  while (!main_window.should_close()) {
    main_window.poll_events();
    main_window.clear_buffers();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    main_window.swap_buffers();
  }

  return 0;
}

// void on_mouse(GLFWwindow *window, double x, double y) {
// xen::camera::update_aim(static_cast<float>(x), static_cast<float>(y), 0.1f);
// }
