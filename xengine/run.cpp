#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "jobsys.h"
#include "checkerr.h"
#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "light.h"

xen::camera::Camera camera;

bool firstMouseMovement = true;
const float cameraAngle = 10.0f;
const float cameraDist = 3.0f;
const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

// keyboard input flags
bool w = false;
bool a = false;
bool s = false;
bool d = false;

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
    xen::Window window(1080, 600);
	window.set_cursor_position_callback(on_mouse);
    xen::ThreadPool threadPool;

	// model shader and model
	auto shader = xen::shader::load("assets/shaders/model.vert", "assets/shaders/model.frag");
	xen::model::Model model;
	xen::model::load(model, "assets/models/cyborg/cyborg.obj");
	xen::model::gen_buffers(model);	// all buffer gen functions must be sequential
	
	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	xen::camera::update_aim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);
	auto viewMatrix = xen::camera::view_matrix(camera);
	auto projectionMatrix = window.projection_matrix(55.0f);
	auto modelMatrix = xen::model::model_matrix(model);

	float currentFrame = 0.0f;

    // TODO - why are jobs not happening?!?!

	// producer loop
	while (!window.should_close())
	{
        window.clear();

		// delta time 
        threadPool.push([&]{
            currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        });
        
        // TODO - this depends on input!!
        threadPool.push([&]{ xen::camera::update_aim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f); });

		// input
		threadPool.push([&]{ window.processKeyInput(w, a, s, d); });

		threadPool.push([&]{ xen::model::process_movement(model, camera.b, w, a, s, d, deltaTime); });
		threadPool.push([&]{ xen::model::update_vectors(model); });

		// render matrices
	    threadPool.push([&]{ viewMatrix = xen::camera::view_matrix(camera); });
		threadPool.push([&]{ projectionMatrix = window.projection_matrix(55.0f); });
		threadPool.push([&]{ modelMatrix = xen::model::model_matrix(model); });

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
		window.swap_and_poll();

		checkerr();
	}
    
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
