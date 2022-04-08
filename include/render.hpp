#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "model.hpp"
#include "camera.hpp"

namespace xen {

    /**
     * @brief Rendering manager for OpenGL
     * 
     */
    class RenderManager {

        // projection matrix
        glm::mat4 projection;

        int scr_width;
        int scr_height;

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
        void buffer_model(Model &model);

        // TODO - replace model with xen::Renderable?
        void draw(Model &model, Shader &shader);

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