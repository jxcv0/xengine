#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * @brief A basic light source.
 *         TODO all of it.
 */
struct Light {
  glm::vec3 m_colour = glm::vec3(1.0f);
  glm::vec3 m_position;
  float m_constant = 1.0f;
  float m_linear = 0.09f;
  float m_quadratic = 0.032f;
};

/**
 * @brief A transformation component.
 */
struct Transform {
  glm::vec3 m_position;
  glm::quat m_rotation;
  glm::vec3 m_scale;
};

#endif  // TYPES_H_
