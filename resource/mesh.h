#ifndef MESH_H_
#define MESH_H_

#include <cstdint>
#include <vector>

#include "vertex.h"
#include "material.h"

/**
 * @brief A Mesh component. This may in fact be a collection of different
 *        Meshes from the same directory.
 */
struct Mesh {
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<Material> m_materials;
};

#endif  // MESH_H_
