#include "render.hpp"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <iostream>

namespace xen {
    /**
     * @brief Initialize the render manager
     * 
     */
    void RenderManager::start_up() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        scr_width = 1600;
        scr_width = 900;

        window = glfwCreateWindow(1600, 900, "xengine", NULL, NULL);
        if (!window) {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window,
            [](GLFWwindow *window, int width, int height){ glViewport(0, 0, width, height); });

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }


    /**
     * @brief Shut down the render manager
     * 
     */
    void RenderManager::shut_down() {
        glfwTerminate();
    }

    bool RenderManager::should_close() {
        return glfwWindowShouldClose(window);
    }

    GLFWwindow* RenderManager::window_ptr() {
        return window;
    }

    /**
     * @brief Swap buffers and poll events
     * 
     */
    void RenderManager::swap_and_poll() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    void RenderManager::set_cursor_pos_callback(GLFWcursorposfun fun) {
        glfwSetCursorPosCallback(window, fun);
    }

    void RenderManager::buffer_mesh(Model::Mesh &mesh) {
        // generate buffers
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        // bind and buffer data
        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Model::Vertex), &mesh.vertices[0],
            GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0],
            GL_STATIC_DRAW);
        
        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
            (void*)offsetof(Model::Vertex, Model::Vertex::normal));

        // texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
            (void*)offsetof(Model::Vertex, Model::Vertex::tex_coords));
        
        // unbind
        glBindVertexArray(0);
    }

    // TODO - replace model with Renderable?
    void RenderManager::draw_mesh(Model::Mesh &mesh, Shader &shader) {

        // TODO - textures
        glBindVertexArray(mesh.VAO);
        glDrawElements(
            GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}; 