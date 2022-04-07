#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shaders.hpp"
#include "util.hpp"
#include "model.hpp"
#include "render.hpp"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 800

xen::RenderManager render_manager;

int main(int argc, char const *argv[]) {

    render_manager.start_up();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // renderable objs to be made up of shaders and models?
    auto shader = xen::load_shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    
    auto model = xen::load_model("assets/models/female_base.obj");

    // view pos = projection . view . global . local

    // projection matrix
    glm::mat4 projection =
    glm::perspective( glm::radians(50.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    // global position matrix
    glm::mat4 global(1.0f);
    global = glm::rotate(global, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    while (!render_manager.should_close()) {
        // input
        xen::util::process_input(render_manager.window_ptr());

        // background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", view);
        shader.set_mat4("global", global);

        xen::draw_model(model, shader);

        // swap and poll
        glfwSwapBuffers(render_manager.window_ptr());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}