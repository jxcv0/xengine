#ifndef VERTEX_H_
#define VERTEX_H_

#include <vec2.h>
#include <vec3.h>

/**
 * @brief A single vertex.
 */
struct Vertex {
  Vec3 m_position;
  Vec3 m_normal;
  Vec2 m_tex_coord;
};

#endif  // VERTEX_H_
