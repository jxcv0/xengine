#ifndef CHARACTER_H
#define CHARACTER_H

namespace xen
{
	// playable character state
	struct Character
	{
		glm::vec3 x;		// local x axis (right)
		glm::vec3 y;		// local y axis (up)
		glm::vec3 z;		// local z axis (front)
		Model model;		// model data TODO multiple for equipables?
	};
} // namespace xen

#endif // CHARACTER_H
