#include <iostream>

#include <mainwindow.h>

int main(int argc, char const *argv[]) {
  MainWindow main_window(1080, 600, std::string("main-window"));
  main_window.set_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  main_window.set_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
  main_window.set_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // main_window.set_cursor_position_callback(on_mouse);
  main_window.show();

  while (!main_window.should_close()) {
    main_window.update_input();
    if (main_window.key_pressed(GLFW_KEY_ESCAPE)) {
      break;
    } else if (main_window.key_pressed(GLFW_KEY_W)) {
      std::cout << "W\n";
    } else if (main_window.key_pressed(GLFW_KEY_A)) {
      std::cout << "A\n";
    }
    main_window.poll_events();
    main_window.clear_buffers();
    main_window.swap_buffers();
  }

  return 0;
}

// void on_mouse(GLFWwindow *window, double x, double y) {
// xen::camera::update_aim(static_cast<float>(x), static_cast<float>(y), 0.1f);
// }
