#ifndef LIGHT_H
#define LIGHT_H

#include <glad.h>

#include <glm/glm.hpp>

namespace xen
{
	struct Light
	{
		glm::vec3 colour = glm::vec3(1.0f);
		glm::vec3 position;
	};
}
#endif // LIGHT_H
