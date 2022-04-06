#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // renderable objs to be made up of shaders and models?
    auto shader = xen::load_shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    auto female_model = xen::load_model("assets/models/female_base.obj");

    // view pos = projection . view . global . local

    // projection matrix
    glm::mat4 projection =
    glm::perspective( glm::radians(50.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    // global position matrix
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -30.0f));

    while (!glfwWindowShouldClose(window)) {
        // input
        xen::util::process_input(window);

        // background
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", view);
        shader.set_mat4("model", model);

        xen::draw_model(female_model, shader);

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}