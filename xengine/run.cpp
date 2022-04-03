#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "shaders.hpp"
#include "util.hpp"

int main(int argc, char const *argv[]) {
    
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
    xen::shader basic_shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.0f, -0.5f, 0.0f, // bottom centre
         0.5f, -0.5f, 0.0f, // bottom right
         0.25f, 0.0f, 0.0f, // top right
         0.0f,  0.5f, 0.0f, // top centre
        -0.25f, 0.0f, 0.0f, // top left
    };

    unsigned int indices[] = {
        0, 1, 5,    // bl
        1, 2, 3,    // br
        3, 4, 5     // top
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        // input
        xen::util::process_input(window);

        // background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        float t = glfwGetTime();
        float green = (sin(t) / 2.0f) + 0.5f;
        int uni_loc = glGetUniformLocation(basic_shader.id, "uni_col");
        basic_shader.use();
        glUniform4f(uni_loc, 0.0f, green, 0.0f, 1.0f);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // dealloc
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    basic_shader.del();

    glfwTerminate();
    return 0;
}
