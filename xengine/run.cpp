#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "alloc.h"
#include "jobsys.h"
#include "checkerr.h"
#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "light.h"

xen::window::Window window;
xen::camera::Camera camera;
bool firstMouseMovement = true;
const float cameraAngle = 10.0f;
const float cameraDist = 3.0f;
const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

// job system
xen::jobs::JobSystemMgr jobSys;

// keyboard input flags
bool w = false;
bool a = false;
bool s = false;
bool d = false;

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::window::init(window, 1080, 600);
	xen::window::set_cursor_position_callback(window, on_mouse);
	jobSys.start_up();
    xen::mem::Allocator allocator<float>(10);

	// model shader and model
	auto shader = xen::shader::load("assets/shaders/model.vert", "assets/shaders/model.frag");
	xen::model::Model model;
	xen::model::load_model(model, "assets/models/cyborg/cyborg.obj");
	xen::model::gen_buffers(model);	// all buffer gen functions must be sequential
	
	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	xen::camera::update_aim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);
	auto viewMatrix = xen::camera::view_matrix(camera);
	auto projectionMatrix = xen::window::projection_matrix(window, 55.0f);
	auto modelMatrix = xen::model::model_matrix(model);

	// producer loop
	while (!xen::window::should_close(window))
	{
		// delta time 
		float currentFrame;
        jobSys.push_job([&] {
            currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        });

		// input
		xen::window::esc(window);
		xen::window::processKeyInput(window, w, a, s, d);
		xen::model::process_movement(model, camera.b, w, a, s, d, deltaTime);
		xen::model::update_vectors(model);

		// background
        // jobSys.push_job([]{ xen::window::bg(0.1f, 0.1f, 0.1f, 1.0f); });
		xen::window::bg(0.1f, 0.1f, 0.1f, 1.0f);

		// render matrices
		jobSys.push_job([&]{ viewMatrix = xen::camera::view_matrix(camera); });
		jobSys.push_job([&]{ projectionMatrix = xen::window::projection_matrix(window, 55.0f); });
		jobSys.push_job([&]{ modelMatrix = xen::model::model_matrix(model); });

		// shader and shader uniforms
		xen::shader::use(shader);
		xen::shader::set_uniform(shader, "view", viewMatrix);
		xen::shader::set_uniform(shader, "model", modelMatrix);
		xen::shader::set_uniform(shader, "projection", projectionMatrix);

		// light
		xen::shader::set_uniform(shader, "viewPosition", camera.position);
		xen::shader::set_uniform(shader, "shininess", 32.0f);
		xen::shader::set_uniform(shader, "light.position", light.position);
		xen::shader::set_uniform(shader, "light.colour", light.colour);
		xen::shader::set_uniform(shader, "light.constant", light.constant);
		xen::shader::set_uniform(shader, "light.linear", light.linear);
		xen::shader::set_uniform(shader, "light.quadratic", light.quadratic);

		// TODO - texture uniforms are assigned when loading, should all uniforms be in the same place?
		xen::model::draw(model, shader);
		xen::window::swap_and_poll(window);

		checkerr();

		w = false;
		a = false;
		s = false;
		d = false;
	}

	jobSys.shut_down();
	xen::window::terminate();

	return 0;
}

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouseMovement)
	{
		camera.xLast = xPos;
		camera.yLast = yPos;
		firstMouseMovement = false;
	}

	float xOffset = xPos - camera.xLast;
	float yOffset = camera.yLast - yPos;
	camera.xLast = xPos;
	camera.yLast = yPos;

	xen::camera::update_aim(camera, cameraAngle, cameraDist, xOffset, yOffset, 0.1f);
}
