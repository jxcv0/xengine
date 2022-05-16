#define XEN_DEBUG

#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "alloc.h"
#include "gamestate.h"
#include "jobsys.h"
#include "checkerr.h"
#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "light.h"

const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
    xen::window::init(1080, 600);
    xen::Input input;
    xen::window::set_cursor_position_callback(on_mouse);
    xen::jobsys::init();

    // models
	auto shader = xen::shader::load_shader("assets/shaders/model.vert", "assets/shaders/model.frag");

    auto model = xen::scene::load_model("assets/models/cyborg/cyborg.obj");
    xen::scene::gen_buffers(model);
	
	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	xen::camera::update_aim(0.0f, 0.0f, 0.1f);
    xen::camera::set_target(xen::scene::model_position(model));
	auto viewMatrix = xen::camera::view_matrix();
	auto projectionMatrix = xen::window::projection_matrix(55.0f);

	float currentFrame = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!xen::window::should_close())
	{
        xen::window::clear();

        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        xen::camera::update_aim(0.0f, 0.0f, 0.1f);

        // input
        auto inputBits = xen::window::get_input();
        // input.set(inputBits);
        
		// render matrices
        auto viewMatrix = xen::camera::view_matrix();

        // this works!
        // xen::jobsys::push([](void* c) -> void* {
        //     auto *camera = static_cast<xen::camera::Camera*>(c);
        //     // do things ...
        // }, (void*)0);

		// shader and shader uniforms
        xen::shader::use_shader(shader);
		xen::shader::set_uniform(shader, "view", viewMatrix);
		xen::shader::set_uniform(shader, "projection", projectionMatrix);

		// light
		xen::shader::set_uniform(shader, "viewPosition", xen::camera::camera_position());
		xen::shader::set_uniform(shader, "shininess", 16.0f);
		xen::shader::set_uniform(shader, "light.position", light.position);
		xen::shader::set_uniform(shader, "light.colour", light.colour);
		xen::shader::set_uniform(shader, "light.constant", light.constant);
		xen::shader::set_uniform(shader, "light.linear", light.linear);
		xen::shader::set_uniform(shader, "light.quadratic", light.quadratic);

        // TODO - this is the kind of thing that should be sent to the render thread
        // OpenGL calls must be single threaded
        xen::scene::update_model_matrix(model);
        xen::shader::set_uniform(shader, "model", xen::scene::model_matrix(model));
        xen::scene::draw_model(model, shader);

        xen::window::swap_buffers();
        xen::window::poll_events();

		checkerr();
        input.clear();
	}

    xen::jobsys::terminate();
	return 0;
}

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn)
{
	xen::camera::update_aim(static_cast<float>(xPosIn), static_cast<float>(yPosIn), 0.1f);
}
