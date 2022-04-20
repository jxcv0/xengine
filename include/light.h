#ifndef LIGHT_H
#define LIGHT_H

#include <glad.h>

#include <glm/glm.hpp>

// looking a little bit empty at the moment
namespace xen
{
	struct Light
	{
		glm::vec3 colour = glm::vec3(1.0f);
		glm::vec3 position;

		// attenuation
		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};
}
#endif // LIGHT_H
