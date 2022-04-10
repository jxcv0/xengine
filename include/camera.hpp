#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace xen {
    class Camera {
        float last_x;
        float last_y;

        float yaw;
        float pitch;

        // camera normals - TODO - pos = bound_character.pos + offset
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        friend void process_cursor_movement(GLFWwindow *window, double x_in, double y_in) {
            // TODO ...
        }

        /**
         * @brief Recalculate the cameras state data
         * 
         */
        void update();

    public:

        Camera() { /* do nothing */ }
        ~Camera() { /* do nothing */ }

        void startUp() {
            yaw = -90.0f;
            pitch = 0.0f;
        }
        
        /**
         * @brief Cache a cursor position
         * 
         * @param x 
         * @param y 
         */
        void set_last(float x, float y);

        /**
         * @brief Process keyboard input
         * 
         * @param window 
         */
        void process_input(GLFWwindow *window);

        /**
         * @brief Process mouse input
         * 
         * @param x 
         * @param y 
         */
        void process_mouse_input(float x, float y);

        /**
         * @brief Calculate the view matrix based on camera position
         * 
         * @return glm::mat4 view matrix
         */
        glm::mat4 view_matrix();
    };

    void process_cursor_movement(GLFWwindow* window, double x_in, double y_in);
} // namespace xen

#endif // _CAMERA_HPP_