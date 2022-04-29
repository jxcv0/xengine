#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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
std::vector<std::thread> threadPool;
std::list<xen::jobsys::Job> xen::jobsys::GLOBAL_JOB_QUEUE;
std::atomic_flag xen::jobsys::GLOBAL_LK = ATOMIC_FLAG_INIT;
std::atomic<bool> xen::jobsys::GLOBAL_RUN = true;

// mock job TODO rm
glm::mat4 viewMatrix;
xen::jobsys::ExitStatus viewMatrixJobFunc(void* camera)
{
	xen::camera::Camera *c = static_cast<xen::camera::Camera*>(camera);
	if (firstMouseMovement) { return xen::jobsys::PAUSE; }

	viewMatrix = xen::camera::viewMatrix(*c);
	return xen::jobsys::SUCCESS;
}
xen::jobsys::Job viewMatrixJob = { viewMatrixJobFunc, (void*)&camera };

// keyboard input flags
bool w = false;
bool a = false;
bool s = false;
bool d = false;

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
	xen::window::init(window, 1080, 600);
	xen::window::setCursorPositionCallback(window, mouseCallback);
	xen::jobsys::initThreadPool(threadPool, 1, xen::jobsys::spin);

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
	viewMatrix = xen::camera::viewMatrix(camera);

	// producer loop
	while (!xen::window::shouldClose(window))
	{
		camera.targetPosition = model.position;

		// delta time 
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	// should be at end of game loop?

		// input
		xen::window::processEsc(window.ptr);
		xen::window::processKeyInput(window, w, a, s, d);
		xen::processModelMovement(model, camera.b, w, a, s, d, deltaTime);
		xen::updateModelVectors(model);

		// background
		xen::window::bg(0.1f, 0.1f, 0.1f, 1.0f);

		// render matrices
		xen::jobsys::pushJob(xen::jobsys::GLOBAL_JOB_QUEUE, viewMatrixJob, &xen::jobsys::GLOBAL_LK);

		// auto viewMatrix = xen::viewMatrix(camera);
		auto projectionMatrix = xen::window::projectionMatrix(window, 55.0f);
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
		xen::window::swapThenPoll(window);

		checkerr();

		w = false;
		a = false;
		s = false;
		d = false;
	}

	xen::jobsys::GLOBAL_RUN = false;
	xen::jobsys::joinThreadPool(threadPool);	
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
