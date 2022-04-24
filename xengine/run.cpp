#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "checkerr.h"
#include "model.h"
#include "camera.h"
#include "window.h"
#include "shader.h"
#include "light.h"

xen::Window window;
xen::Camera camera;
bool firstMouseMovement = true;
const float cameraAngle = 10.0f;
const float cameraDist = 3.0f;

// keyboard input flags
bool w = false;
bool a = false;
bool s = false;
bool d = false;

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::initWindow(window, 1080, 600);
	xen::setCursorPositionCallback(window, mouseCallback);

	// model shader and model
	auto shader = xen::loadShaderFromFile("assets/shaders/model.vert", "assets/shaders/model.frag");
	xen::Model model;
	xen::loadModel(model, "assets/models/cyborg/cyborg.obj");
	xen::genModelBuffers(model);	// all buffer gen functions must be sequential

	// 3rd person camera
	// xen::updateCameraAim(camera, 75.0f, 3.16f, 0.0f, 0.0f, 0.01f);
	camera.position = model.position;
	camera.position.y = 3.5f;

	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	xen::updateCameraAim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);
	std::cout << "xyz: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << "\n";
	std::cout << "ab: " << camera.a << " " << camera.b << "\n";

	while (!xen::windowShouldClose(window))
	{
		// delta time 
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	// should be at end of game loop?
		
		// input
		xen::processEsc(window.ptr);
		xen::processKeyInput(window, w, a, s, d);
		xen::processModelMovement(model, camera.z, w, a, s, d, deltaTime);
		
		// background
		xen::fill(0.1f, 0.1f, 0.1f, 1.0f);
		xen::clear();

		// render matrices
		auto viewMatrix = xen::viewMatrix(camera);
		auto projectionMatrix = xen::projectionMatrix(window, 55.0f);
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

		checkerr();

		w = false;
		a = false;
		s = false;
		d = false;
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

	// xen::updateCameraAim(camera, xOffset * 0.1f, yOffset * 0.1f);
	xen::updateCameraAim(camera, cameraAngle, cameraDist, xOffset, yOffset, 0.1f);
	std::cout << "xyz: " << camera.position.x << " " << camera.position.y << " " << camera.position.z << "\n";
	std::cout << "ab: " << camera.a << " " << camera.b << "\n";
}
