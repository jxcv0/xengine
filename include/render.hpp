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


        GLFWwindow* window;

    public:
        int scr_width;
        int scr_height;

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
        void poll_events();

        void swap_buffers();

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
        void buffer_model(Model &model);

        // TODO - replace model with xen::Renderable?
        void draw_mesh(Model::Mesh &mesh, Shader &shader);

        void draw_model(Model &model, Shader &shader);

        void fill_bg(glm::vec4 col);
        void fill_bg();
    };
} // namespace xen
#endif // _RENDER_HPP_