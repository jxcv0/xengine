#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "model.hpp"
#include "shaders.hpp"

namespace xen {

    /**
     * @brief Rendering manager for OpenGL
     * 
     */
    class RenderManager {

        int scr_width = 1000;
        int scr_height = 800;

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
        void start_up();

        /**
         * @brief Shut down the render manager
         * 
         */
        void shut_down();

        /**
         * @brief Generate a projection matrix
         * 
         * @return glm::mat4 
         */
        glm::mat4 projection_matrix();

        /**
         * @brief Check if window should close
         * 
         * @return true 
         * @return false 
         */
        bool should_close();

        /**
         * @brief Get a pointer to the window
         * 
         * @return GLFWwindow* 
         */
        GLFWwindow* window_ptr();

        /**
         * @brief Swap buffers and poll events
         * 
         */
        void swap_and_poll();

        /**
         * @brief Set mouse callback function
         * 
         * @param mouse_callback 
         */
        void set_cursor_pos_callback(GLFWcursorposfun mouse_callback);

        /**
         * @brief Add model data to gl buffers
         * 
         * @param model 
         */
        void buffer_mesh(Model::Mesh &mesh);

        /**
         * @brief Add model data to gl buffers
         * 
         * @param model 
         */
        void buffer_model(Model &model) {
            for (size_t i = 0; i < model.meshes.size(); i++) {
                buffer_mesh(model.meshes[i]);
            }
        }

        // TODO - replace model with xen::Renderable?
        void draw_mesh(Model::Mesh &mesh, Shader &shader);

        void render_model(Model &model, Shader &shader) {
            for (size_t i = 0; i < model.meshes.size(); i++) {
                draw_mesh(model.meshes[i], shader);
            }
        }

        void fill_bg(glm::vec4 col = glm::vec4(0.01f, 0.02f, 0.02f, 1.0f)) {
            glClearColor(col.x, col.y, col.z, col.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };
} // namespace xen
#endif // _RENDER_HPP_