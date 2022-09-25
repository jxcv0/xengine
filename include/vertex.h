#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/ext.hpp>
#include <glm/glm.hpp>

/**
 * @brief A single vertex.
 */
struct Vertex {
  glm::vec3 m_position;
  glm::vec3 m_normal;
  glm::vec2 m_tex_coord;
  glm::vec3 m_tangent;
  glm::vec3 m_bitangent;
};

#endif  // VERTEX_H_
