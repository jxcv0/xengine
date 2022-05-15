#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace
{
    glm::vec3 _targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);	// world position
    glm::vec3 _x = glm::vec3(0.0f, 1.0f, 0.0f);						// local x axis (right)
    glm::vec3 _y = glm::vec3(0.0f, 1.0f, 0.0f);		// local y axis (up)
    glm::vec3 _z = glm::vec3(0.0f, 0.0f, -1.0f);		// local z axis (front)
    float _a = 0.0f;						// rotation around local x axis (pitch)
    float _b = 90.0f;					// rotation around local y axis (yaw)
    float _xLast = 0;
    float _yLast = 0;
    bool firstMouseMovement = true;
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

    // TODO - fix!!
	// update _vectors and position based on a centre of rotation about an xz position
	void update_aim(float x, float y, float sen)
	{
        if (firstMouseMovement)
        {
            _xLast = x;
            _yLast = y;
            firstMouseMovement = false;
        }

        float xOffset = x - _xLast;
        float yOffset = _yLast - y;

        _xLast = xOffset;
        _yLast = yOffset;

		_a += xOffset * sen;
		_b += yOffset * sen;

		if (_a > 50.0f) { _a = 50.0f; };
		if (_a < -50.0f) { _a = -50.0f; };

		_position.x = _targetPosition.x + cos(glm::radians(_b));
		_position.y = _targetPosition.y + sin(glm::radians(_a)) + 3.5f;
		_position.z = _targetPosition.z + sin(glm::radians(_b));	// this isnt quite right

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
