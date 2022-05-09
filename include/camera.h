#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace xen::camera
{
	// camera state
	struct Camera
	{
		glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);	// world position
		glm::vec3 x;						// local x axis (right)
		glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);		// local y axis (up)
		glm::vec3 z = glm::vec3(0.0f, 0.0f, -1.0f);		// local z axis (front)
		float a = 0.0f;						// rotation around local x axis (pitch)
		float b = 90.0f;					// rotation around local y axis (yaw)
		float xLast;
		float yLast;
	};

	// get view matrix from camera state
	glm::mat4 view_matrix(Camera &camera)
	{
		return glm::lookAt(camera.position, camera.position + camera.z, camera.y);
	}

	// update camera vectors with mouse position
	void update_aim(Camera &camera, float x, float y)
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

	// update camera vectors and position based on a centre of rotation about an xz position
	void update_aim(Camera &camera, float offsetAnglexz, float offsetDist, float x, float y, float sen)
	{
		camera.a += y * sen;
		camera.b += x * sen;

		if (camera.a > 50.0f) { camera.a = 50.0f; };
		if (camera.a < -50.0f) { camera.a = -50.0f; };

		camera.position.x = camera.targetPosition.x + (-offsetDist * cos(glm::radians(camera.b)));
		camera.position.y = camera.targetPosition.y + (-offsetDist * sin(glm::radians(camera.a))) + 3.5f;
		camera.position.z = camera.targetPosition.z + (-offsetDist * sin(glm::radians(camera.b)));	// this isnt quite right

		camera.z = glm::normalize(glm::vec3(
			cos(glm::radians(camera.b)) * cos(glm::radians(camera.a)),
			sin(glm::radians(camera.a)),
			sin(glm::radians(camera.b)) * cos(glm::radians(camera.a))));

		camera.x = glm::normalize(glm::cross(camera.z, camera.y));
	}

	// update camera position based on key press / character movement flags
    template<typename Input>
	void process_movement(Camera &camera, Input in, float deltaTime)
	{
		float velocity = 2.5f * deltaTime;
		if (in.forward()) { camera.position += camera.z * velocity; }
		if (in.backward()) { camera.position -= camera.z * velocity; }
		if (in.left()) { camera.position -= camera.x * velocity; }
		if (in.right()) { camera.position += camera.x * velocity; }
	}
} // namespace xen::camera

#endif // CAMERA_H
