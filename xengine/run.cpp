#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shaders.hpp"
#include "util.hpp"
#include "model.hpp"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 800

int main(int argc, char const *argv[]) {

    // init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "TEST", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, xen::util::framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;;
        return -1;
    }

    // shaders
    auto basic_shader = xen::load_shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // view pos = projection . view . global . local

    // projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(50.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    // global position matrix
    glm::mat4 global(1.0f);
    global = glm::rotate(global, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


    while (!glfwWindowShouldClose(window)) {
        // input
        xen::util::process_input(window);

        // background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
