#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <cstdint>

#include "vertex.h"

/**
 * @brief A Mesh component. This may in fact be a collection of different
 *        Meshes from the same directory.
 */
struct Mesh {
  std::vector<Vertex> m_vertices;
  std::vector<std::uint32_t> m_indices;
};

#endif // MESH_H_
