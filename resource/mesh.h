#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include "material.h"
#include <vec3.h>
#include <vec2.h>

/**
 * @brief A Mesh component. This may in fact be a collection of different
 *        Meshes from the same directory.
 */
struct Mesh {
  std::vector<Vec3> m_positions;
  std::vector<Vec3> m_normals;
  std::vector<Vec2> m_tex_coords;
  std::vector<unsigned int> m_indices;
  std::vector<Material> m_materials;
};

#endif  // MESH_H_
