#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <omp.h>

#include "assets.h"
#include "glad.h"
#include "window.h"

#define COUNT 64

struct mesh meshes[COUNT] = {0};
GLFWwindow *window;

int main() {
  create_window(&window, 640, 480, "temp window");

  for (int i = 0; i < COUNT; i += omp_get_num_threads()) {
    meshes[i] = load_mesh("cyborg.model");
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
