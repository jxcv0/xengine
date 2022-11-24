#include <glad.h>
#include <lin.h>
#include <mainwindow.h>
#include <shader.h>
#include <vec3.h>
#include <checkerr.h>

// #include <mesh.h>


#include <iostream>

// TODO copy learnopengl code for cube here then replace with Resource<Mesh>

int main(int argc, char const *argv[]) {
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

  float vertices[] = {
    0.5f, -0.5f, -0.5f,
    0.5f,  -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f,  0.5f,  -0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f
  };

  /*
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  */

  unsigned int vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // TODO set up buffers for cube
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  auto projection_matrix = main_window.projection_matrix(60.0f);
  auto view_matrix = lin::translate(Mat4(1.0f), Vec3(0.0f, 0.0f, -3.0f));
  auto model_matrix = lin::translate(Mat4(1.0f), Vec3(0.0f, 0.0f, 0.0f));

  shader.set_uniform("projection", projection_matrix);
  shader.set_uniform("view", view_matrix);
  shader.set_uniform("model", model_matrix);
  shader.use();

  while (!main_window.should_close()) {
    main_window.poll_events();
    main_window.clear_buffers();

    glDrawArrays(GL_TRIANGLES, 0, 8);

    main_window.swap_buffers();
  }

  return 0;
}

// void on_mouse(GLFWwindow *window, double x, double y) {
// xen::camera::update_aim(static_cast<float>(x), static_cast<float>(y), 0.1f);
// }
