#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"

xen::Window mainWindow;
xen::Camera camera;
bool firstMouseMovement = true;

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::initWindow(mainWindow);
	xen::setCursorPositionCallback(mainWindow, mouseCallback);
	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

	auto shader = xen::loadShaderFromFile("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	auto texture = xen::loadTextureFromFile("assets/textures/stone_blocks.jpg");
	xen::Model femaleModel;
	xen::loadModel(femaleModel, "assets/models/female_base/test/female_base_texture_test.obj");
	xen::genModelBuffers(femaleModel);

	while (!xen::windowShouldClose(mainWindow))
	{
		// background
		xen::fill(0.3f, 0.1f, 0.1f, 1.0f);
		xen::clear();

		// render matrices
		auto viewMatrix = xen::viewMatrix(camera);
		auto projectionMatrix = xen::projectionMatrix(mainWindow);
		auto modelMatrix = xen::modelMatrix(femaleModel);

		// shader and shader uniforms
		xen::useShader(shader);
		xen::setShaderUniform(shader, "model", modelMatrix);
		xen::setShaderUniform(shader, "view", viewMatrix);
		xen::setShaderUniform(shader, "projection", projectionMatrix);

		// input
		xen::processKeyPress(mainWindow.ptr);

		// model must be drawn after calling xen::clear
		xen::setShaderUniform(shader, "model", modelMatrix);
		xen::drawModel(femaleModel);

		xen::swapThenPoll(mainWindow);
	}
	
	glfwTerminate();

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
