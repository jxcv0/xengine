#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shaders.hpp"
#include "model.hpp"
#include "render.hpp"
#include "camera.hpp"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 800

xen::Camera CAMERA;
xen::RenderManager RENDERMANAGER;

int main(int argc, char const *argv[]) {
    RENDERMANAGER.start_up();
    CAMERA.startUp();
    
    RENDERMANAGER.set_cursor_pos_callback(xen::process_cursor_movement);
    CAMERA.set_last((float)WINDOW_WIDTH / 2.0, (float)WINDOW_HEIGHT / 2.0);

    // renderable objs to be made up of shaders and models?
    auto shader = xen::load_shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    xen::Model model; 
    model.import("assets/models/rock/rock.obj");
    // model.import("assets/models/female_base/test/female_base_texture_test.obj");
    model.position = glm::vec3(0.0f);
    RENDERMANAGER.buffer_model(model);

    // view pos = projection . view . global . local

    // global position matrix
    glm::mat4 global(1.0f);
    global = glm::rotate(global, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    while (!RENDERMANAGER.should_close()) {
        CAMERA.process_input(RENDERMANAGER.window_ptr());
        RENDERMANAGER.fill_bg();
        shader.use();

        shader.set_mat4("projection", RENDERMANAGER.projection_matrix());
        shader.set_mat4("view", CAMERA.view_matrix());
        shader.set_mat4("model", model.model_matrix());
        RENDERMANAGER.draw_model(model, shader);
        RENDERMANAGER.swap_and_poll();
    }

    glfwTerminate();
    return 0;
}
