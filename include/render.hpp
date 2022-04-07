#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "model.hpp"

namespace xen {

    /**
     * @brief Rendering manager for OpenGL
     * 
     */
    class RenderManager {

        GLFWwindow* window;

    public:
    

        RenderManager() {
            // do nothing ...
        }

        ~RenderManager() {
            // do nothing ...
        }

        /**
         * @brief Initialize the render manager
         * 
         */
        void start_up() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
                std::cout << "Failed to initialize GLAD" << std::endl;;
            }
        }

        /**
         * @brief Shut down the render manager
         * 
         */
        void shut_down() {
            glfwTerminate();
        }

        bool should_close() {
            return glfwWindowShouldClose(window);
        }

        GLFWwindow* window_ptr() {
            return window;
        }

        void register_model(Model &model) {
            // generate buffers
            glGenVertexArrays(1, &model.VAO);
            glGenBuffers(1, &model.VBO);
            glGenBuffers(1, &model.EBO);

            // bind and buffer data
            glBindVertexArray(model.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
            glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Model::Vertex), &model.vertices[0],
                GL_STATIC_DRAW);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), &model.indices[0],
                GL_STATIC_DRAW);
            
            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)0);

            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
                (void*)offsetof(Model::Vertex, Model::Vertex::norm));

            // texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
                (void*)offsetof(Model::Vertex, Model::Vertex::tex_coords));
            
            // unbind
            glBindVertexArray(0);
        }

        // TODO - replace model with xen::Renderable
        void draw(Model &model, Shader &shader) {

            // TODO - textures

            glBindVertexArray(model.VAO);
            glDrawElements(
                GL_TRIANGLES, static_cast<unsigned int>(model.indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // unsigned int xen::load_texture(const char *path) {

        //     unsigned int tex_id;
        //     glGenTextures(1, &tex_id);

        //     stbi_set_flip_vertically_on_load(true);

        //     int w, h, no;
        //     unsigned char *data = stbi_load(path, &w, &h, &no, 0);

        //     if (data) {
        //         GLenum format;
        //         if (no == 1) {
        //             format = GL_RED;
        //         } else if (no == 3) {
        //             format = GL_RGB;
        //         } else if (no = 4) {
        //             format = GL_RGBA;
        //         }

        //         glBindTexture(GL_TEXTURE_2D, tex_id);
        //         glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        //         glGenerateMipmap(GL_TEXTURE_2D);

        //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //         stbi_image_free(data);
        //     } else {
        //         std::cout << "Unable to load texture from " << path << std::endl;
        //         stbi_image_free(data);
        //     }

        //     return tex_id;
        // }
    };
} // namespace xen
#endif // _RENDER_HPP_