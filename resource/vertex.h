#ifndef VERTEX_H_
#define VERTEX_H_

#include <vec3.h>
#include <vec2.h>

/**
 * @brief A single vertex.
 */
struct Vertex {
  Vec3 m_position;
  Vec3 m_normal;
  Vec2 m_tex_coord;
  Vec3 m_tangent;
  Vec3 m_bitangent;
};

#endif  // VERTEX_H_
