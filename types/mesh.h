#ifndef MESH_H_
#define MESH_H_

#include <vec2.h>
#include <vec3.h>

#include <vector>

/**
 * @brief A Mesh component.
 */
struct Mesh {
  // Stores the indexes into the vertex data.
  // This makes parsing .obj's much easier at the expense of a slighly more
  // expensive call to glBufferData(GL_ARRAY_BUFFER, ... );
  // This is because the data needs to be decompressed into a separate buffer
  // using this as a key before being passed to OpenGL calls.
  struct Index {
    unsigned int m_position_idx;
    unsigned int m_tex_coord_idx;
    unsigned int m_normal_idx;
  };

  std::vector<Vec3> m_positions;
  std::vector<Vec3> m_normals;
  std::vector<Vec2> m_tex_coords;
  std::vector<Index> m_indices;
};

#endif  // MESH_H_
