#ifndef CAMERA_H
#define CAMERA_H

namespace xen
{
	// camera state
	struct Camera
	{
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
	glm::mat4 viewMatrix(Camera &camera)
	{
		return glm::lookAt(camera.position, camera.position + camera.z, camera.y);
	}

	// update camera vectors with mouse position
	void updateCameraAim(Camera &camera, float x, float y)
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
	void updateCameraAim(Camera &camera, float offsetAnglexz, float offsetDist, float x, float y, float sen)
	{
		camera.a += y * sen;
		camera.b += x * sen;

		if (camera.a > 89.0f) { camera.a = 89.0f; };
		if (camera.a < -89.0f) { camera.a = -89.0f; };

		// assuming ve+ (CW)
		float theta = glm::radians(camera.b - offsetAnglexz);
		camera.position.x = offsetDist * cos(theta);
		camera.position.z = offsetDist * sin(theta);
		camera.position.y = 2.5f;

		glm::vec3 radius;
		radius.x = offsetDist * cos(glm::radians(camera.b));
		radius.z = offsetDist * sin(glm::radians(camera.b));
		radius.y = camera.position.y;

		camera.z = glm::normalize(glm::cross(radius, camera.y));
		// camera.z = glm::normalize(glm::vec4(
			// cos(glm::radians(camera.b)) * cos(glm::radians(camera.a)),
			// sin(glm::radians(camera.a)),
			// sin(glm::radians(camera.b)) * cos(glm::radians(camera.a))));

		camera.x = glm::normalize(glm::cross(camera.z, camera.y));
	}

	// update camera position based on key press / character movement flags
	void processCameraMovement(Camera &camera, bool w, bool a, bool s, bool d, float deltaTime)
	{
		float velocity = 2.5f * deltaTime;
		if (w) { camera.position += camera.z * velocity; }
		if (s) { camera.position -= camera.z * velocity; }
		if (a) { camera.position -= camera.x * velocity; }
		if (d) { camera.position += camera.x * velocity; }
	}
}

#endif // CAMERA_H
