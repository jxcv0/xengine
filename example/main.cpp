#include <glad.h>
#include <mainwindow.h>
#include <shader.h>
// #include <mesh.h>

#include <iostream>

// TODO copy learnopengl code for cube here then replace with Resource<Mesh>

auto get_verts() {
  return new float[9]{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
}

int main(int argc, char const *argv[]) {
  MainWindow main_window(1080, 600, std::string("main-window"));
  main_window.set_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  main_window.set_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
  main_window.set_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // main_window.set_cursor_position_callback(on_mouse);
  main_window.show();  // TODO

  auto shader = ShaderUtils::load("assets/shaders/hellotriangle.vert",
                                  "assets/shaders/hellotriangle.frag");

  unsigned int vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  auto vertices = get_verts();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // delete[] vertices;
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
  delete[] vertices;  // this could be useful
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

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
