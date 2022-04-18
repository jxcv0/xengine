#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "keys.h"
#include "light.h"

xen::Window window;
xen::Camera camera;
bool firstMouseMovement = true;

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::initWindow(window);
	xen::setCursorPositionCallback(window, mouseCallback);
	camera.position = glm::vec3(0.0f, 1.0f, 3.0f);

	auto shader = xen::loadShaderFromFile("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	xen::Model model;
	xen::loadModel(model, "assets/models/cyborg/cyborg.obj");
	xen::genModelBuffers(model);	// all buffer gen functions must be sequential

	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, 1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!xen::windowShouldClose(window))
	{
		// delta time 
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	// should be at end of game loop?

		// input
		xen::processEsc(window.ptr);
		xen::processMovement(camera, xen::processKeyInput(window), deltaTime);

		// background
		xen::fill(0.1f, 0.1f, 0.1f, 1.0f);
		xen::clear();

		// render matrices
		auto viewMatrix = xen::viewMatrix(camera);
		auto projectionMatrix = xen::projectionMatrix(window);
		auto modelMatrix = xen::modelMatrix(model);

		// shader and shader uniforms
		xen::useShader(shader);
		xen::setShaderUniform(shader, "model", modelMatrix);
		xen::setShaderUniform(shader, "view", viewMatrix);
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

		xen::swapThenPoll(window);
	}
	
	xen::terminate();

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

	xen::updateCamera(camera, xOffset * 0.1f, yOffset * 0.1f);
}
