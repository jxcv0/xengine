#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <type_traits>
#include <functional>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "jobsys.h"
#include "checkerr.h"
#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "light.h"

xen::window::Window GLOBAL_WINDOW;
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

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::window::init(GLOBAL_WINDOW, 1080, 600);
	xen::window::setCursorPositionCallback(GLOBAL_WINDOW, mouseCallback);
	jobSys.start_up(2); // only need a few threads for now

	// model shader and model
	auto shader = xen::loadShaderFromFile("assets/shaders/model.vert", "assets/shaders/model.frag");
	xen::Model model;
	xen::loadModel(model, "assets/models/cyborg/cyborg.obj");
	xen::genModelBuffers(model);	// all buffer gen functions must be sequential
	
	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	xen::camera::updateCameraAim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);
	auto viewMatrix = xen::camera::viewMatrix(camera);

	// producer loop
	while (!xen::window::shouldClose(GLOBAL_WINDOW))
	{

		// delta time 
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	// should be at end of game loop?

		// input
		xen::window::esc(GLOBAL_WINDOW);
		xen::window::processKeyInput(GLOBAL_WINDOW, w, a, s, d);
		xen::processModelMovement(model, camera.b, w, a, s, d, deltaTime);
		xen::updateModelVectors(model);

		// background
		xen::window::bg(0.1f, 0.1f, 0.1f, 1.0f);

		// render matrices
		jobSys.push_job([&]{ viewMatrix = xen::camera::viewMatrix(camera); });
		// viewMatrix = xen::camera::viewMatrix(camera);

		// auto viewMatrix = xen::viewMatrix(camera);
		auto projectionMatrix = xen::window::projectionMatrix(GLOBAL_WINDOW, 55.0f);
		auto modelMatrix = xen::modelMatrix(model);

		// shader and shader uniforms
		xen::useShader(shader);
		xen::setShaderUniform(shader, "view", viewMatrix);
		xen::setShaderUniform(shader, "model", modelMatrix);
		xen::setShaderUniform(shader, "projection", projectionMatrix);

		// light
		xen::setShaderUniform(shader, "viewPosition", camera.position);
		xen::setShaderUniform(shader, "shininess", 32.0f);
		xen::setShaderUniform(shader, "light.position", light.position);
		xen::setShaderUniform(shader, "light.colour", light.colour);
		xen::setShaderUniform(shader, "light.constant", light.constant);
		xen::setShaderUniform(shader, "light.linear", light.linear);
		xen::setShaderUniform(shader, "light.quadratic", light.quadratic);

		// TODO - texture uniforms are assigned when loading, should all uniforms be in the same place?
		xen::drawModel(model, shader);
		xen::window::swapThenPoll(GLOBAL_WINDOW);

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

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn)
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

	xen::camera::updateCameraAim(camera, cameraAngle, cameraDist, xOffset, yOffset, 0.1f);
}
