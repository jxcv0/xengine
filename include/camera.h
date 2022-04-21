#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"

namespace xen
{
	// camera state
	struct Camera
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);	// world position
		glm::vec3 x;						// positive x vector (to right of camera)
		glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);		// positive y vector (camera up axis)
		glm::vec3 z = glm::vec3(0.0f, 0.0f, -1.0f);		// camera z vector (the aim vector)
		float a = 0.0f;						// rotation around local x axis (pitch)
		float b = 90.0f;					// rotation around local y axis (yaw)
		float xLast;
		float yLast;
	};

	// get view matrix from camera state
	glm::mat4 viewMatrix(Camera &camera)
	{
		return glm::lookAt(camera.position, camera.position + camera.z, camera.y);
	}

	// update camera vectors with mouse position
	void updateCamera(Camera &camera, float x, float y)
	{
		camera.a += y;
		camera.b += x;

		if (camera.a > 89.0f) { camera.a = 89.0f; };
		if (camera.a < -89.0f) { camera.a = -89.0f; };

		camera.z = glm::normalize(glm::vec3(
			cos(glm::radians(camera.b)) * cos(glm::radians(camera.a)),
			sin(glm::radians(camera.a)),
			sin(glm::radians(camera.b)) * cos(glm::radians(camera.a))));

		camera.x = glm::normalize(glm::cross(camera.z, camera.y));
	}

	void processMovement(Camera &camera, bool w, bool a, bool s, bool d, float deltaTime)
	{
		float velocity = 2.5f * deltaTime;
		if (w)
		{
			camera.position += camera.z * velocity;
		}

		if (s)
		{
			camera.position -= camera.z * velocity;
		}

		if (a)
		{
			camera.position -= camera.x * velocity;
		}

		if (d)
		{
			camera.position += camera.x * velocity;
		}
	}
}

#endif // CAMERA_H

//719148
