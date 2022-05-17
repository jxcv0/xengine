#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace
{
    glm::vec3* _target;
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);	// world position
    glm::vec3 _x = glm::vec3(0.0f, 1.0f, 0.0f);						// local x axis (right)
    glm::vec3 _y = glm::vec3(0.0f, 1.0f, 0.0f);		// local y axis (up)
    glm::vec3 _z = glm::vec3(0.0f, 0.0f, -1.0f);		// local z axis (front)
    float _a = 0.0f;						// rotation around local x axis (pitch)
    float _b = 0.0f;					// rotation around local y axis (yaw)
    float _prevX = 0;
    float _prevY = 0;
    bool _firstMouseMovement = true;
} // namespace

namespace xen::camera
{
	// calculate view matrix
	glm::mat4 view_matrix()
	{
		return glm::lookAt(_position, _position + _z, _y);
	}

    // get camera position
    glm::vec3 camera_position()
    {
        return _position;
    }

    // set the target position of the camera
    void set_target(glm::vec3* target)
    {
        _target = target;
    }

	// update _vectors and position based on a centre of rotation about an xz position
	void update_aim(float mouseX, float mouseY, float sen)
	{
        if (_firstMouseMovement)
        {
            _prevX = mouseX;
            _prevY = mouseY;
            _firstMouseMovement = false;
        }

        auto changeX = mouseX - _prevX;
        auto changeY = _prevY - mouseY;

        _prevX = mouseX;
        _prevY = mouseY;

		_b += changeX * sen;
		_a += changeY * sen;

		if (_a > 50.0f) { _a = 50.0f; };
		if (_a < -50.0f) { _a = -50.0f; };

        // TODO 3rd person
        if (_target)
        {
            _position.x = _target->x * cos(glm::radians(_b));
            _position.y = _target->y * sin(glm::radians(_a)) + 3.5f;
            _position.z = _target->z * sin(glm::radians(_b));
        }

		_z = glm::normalize(glm::vec3(
			cos(glm::radians(_b)) * cos(glm::radians(_a)),
			sin(glm::radians(_a)),
			sin(glm::radians(_b)) * cos(glm::radians(_a))));

		_x = glm::normalize(glm::cross(_z, _y));
	}

	// update _position based on key press / character movement flags
    template<typename Input>
	void process_movement(Input in, float deltaTime)
	{
		float velocity = 2.5f * deltaTime;
		if (in.forward()) { _position += _z * velocity; }
		if (in.backward()) { _position -= _z * velocity; }
		if (in.left()) { _position -= _x * velocity; }
		if (in.right()) { _position += _x * velocity; }
	}
} // namespace xen::camera

#endif // CAMERA_H
