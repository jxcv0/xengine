#ifndef MESH_H_
#define MESH_H_

#include <vec2.h>
#include <vec3.h>

#include <vector>

/**
 * @brief A Mesh component.
 */
struct Mesh {
  struct Index {
    unsigned int m_position_idx;
    unsigned int m_tex_coord_idx;
    unsigned int m_normal_idx;
  };

  void *mp_memory_block;
  Vec3 *mp_positions;
  Vec3 *mp_normals;
  Vec2 *mp_tex_coords;
  Index *mp_indices;
  unsigned int m_num_positions;
  unsigned int m_num_normals;
  unsigned int m_num_tex_coords;
  unsigned int m_num_indices;
};

#endif  // MESH_H_
