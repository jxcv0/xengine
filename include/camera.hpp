#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace xen {
    struct Camera {
        float last_x;
        float last_y;
        bool init = false;

        float yaw;
        float pitch;

        // camera normals
        glm::vec3 pos;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        void update() {
            glm::vec3 f;
            f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            f.y = sin(glm::radians(pitch));
            f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(f);
            right = glm::normalize(glm::cross(front, world_up));
            up = glm::normalize(glm::cross(right, front));
        }
    
    public:

        Camera() {
            // do nothing ...
        }

        ~Camera() {
            // do nothing ...
        }

        bool initialized() {
            return init;
        }

        void set_last(float x, float y) {
            last_x = x;
            last_y = y;
            init = true;
        }

        void process_mouse_input(float x, float y) {

            float x_offset = x - last_x;
            float y_offset = last_x - y;

            last_x = x;
            last_x = y;

            float sensitivity = 0.1f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            yaw *= x_offset;
            pitch *= y_offset;

            if(pitch > 89.0f) {
                pitch = 89.0f;
            }

            if(pitch < -89.0f) {
                pitch = -89.0f;
            }

            update();
        }
    };
} // namespace xen

#endif // _CAMERA_HPP_