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

xen::camera::Camera camera;

bool firstMouseMovement = true;
const float cameraAngle = 10.0f;
const float cameraDist = 3.0f;
const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

void on_mouse(GLFWwindow *window, double xPosIn, double yPosIn);

int main(int argc, char const *argv[])
{
    xen::Window window(1080, 600);
    xen::Input input;
	window.set_cursor_position_callback(on_mouse);
    xen::jobsys::init();

    // models
	auto shader = xen::shader::load("assets/shaders/model.vert", "assets/shaders/model.frag");

    xen::model::Model models[5];
    for (size_t i = 0; i < 5; i++)
    {
        models[i].position.x = i * 4;
        xen::model::load(models[i], "assets/models/cyborg/cyborg.obj");
        xen::model::gen_buffers(models[i]);	// all buffer gen functions must be sequential
        models[i].b =+ 180.0f; 
    }
	
	// temp light
	xen::Light light;
	light.position = glm::vec3(0.0f, 3.0f, -1.0f);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	xen::camera::update_aim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);
	auto viewMatrix = xen::camera::view_matrix(camera);
	auto projectionMatrix = window.projection_matrix(55.0f);

	float currentFrame = 0.0f;

    // TODO producer thread synchronizes pushed work and synchronizes dependant tasks
    xen::mem::StackAllocator<glm::mat4> matrixAllocator(3);

	while (!window.should_close())
	{
        window.clear();

        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
       // xen::camera::update_aim(camera, cameraAngle, cameraDist, 0.0f, 0.0f, 0.1f);

       // input
        auto inputBits = window.get_input();
        input.set(inputBits);
        // xen::model::process_movement(model, camera.b, input, deltaTime);
        // xen::model::update_vectors(model);

        xen::camera::process_movement(camera, input, deltaTime);

		// render matrices
        glm::mat4 *viewMatrix = matrixAllocator.allocate(1);
        glm::mat4 *projectionMatrix = matrixAllocator.allocate(1);
        *viewMatrix = xen::camera::view_matrix(camera);
        *projectionMatrix = window.projection_matrix(55.0f);

		// shader and shader uniforms
		xen::shader::use(shader);
		xen::shader::set_uniform(shader, "view", *viewMatrix);
		xen::shader::set_uniform(shader, "projection", *projectionMatrix);

		// light
		xen::shader::set_uniform(shader, "viewPosition", camera.position);
		xen::shader::set_uniform(shader, "shininess", 16.0f);
		xen::shader::set_uniform(shader, "light.position", light.position);
		xen::shader::set_uniform(shader, "light.colour", light.colour);
		xen::shader::set_uniform(shader, "light.constant", light.constant);
		xen::shader::set_uniform(shader, "light.linear", light.linear);
		xen::shader::set_uniform(shader, "light.quadratic", light.quadratic);

        for (size_t i = 0; i < 5; i++)
        {
            xen::jobsys::push(xen::model::update_model_job, (void*)&models[i]);
        }

        // TODO - this is the kind of thing that should be set to the render thread
        // OpenGL calls must be single threaded
        for (auto &m : models)
        {
            xen::shader::set_uniform(shader, "model", m.matrix);
            xen::model::draw(m, shader);
        }

		window.swap_and_poll();
		checkerr();
        input.clear();
        matrixAllocator.clear();
	}

    xen::jobsys::terminate();
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

    xen::camera::update_aim(camera, xOffset, yOffset, 0.1f);
	// xen::camera::update_aim(camera, cameraAngle, cameraDist, xOffset, yOffset, 0.1f);
}
