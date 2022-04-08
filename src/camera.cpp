#include "camera.hpp"

namespace xen {
    void Camera::update() {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }

    bool Camera::initialized() {
        return init;
    }

    void Camera::set_last(float x, float y) {
        last_x = x;
        last_y = y;
        init = true;
    }

    void Camera::process_input(GLFWwindow *window) {
        // TODO in distant future - make this an atribute of the playable character
        const float movement_speed = 0.05f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pos += movement_speed * front;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pos -= movement_speed * front;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos -= glm::normalize(glm::cross(front, world_up)) * movement_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos += glm::normalize(glm::cross(front, world_up)) * movement_speed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void Camera::process_mouse_input(float x, float y) {

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

    glm::mat4 Camera::view_matrix() {
        return glm::lookAt(pos, pos + front, up);
    }
} // namespace xen