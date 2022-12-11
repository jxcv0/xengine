#ifndef MESH_H_
#define MESH_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <limits>

#include "vec2.h"
#include "vec3.h"

/**
 * @brief Stored mesh data. This class is used to load and
 *        unload from a file using an allocator supplied to it.
 */
class Mesh {
  struct Vertex {
    Vec3 m_position;
    Vec3 m_normal;
    Vec2 m_tex_coord;
  };

 public:
  /**
   * @brief Load mesh data from an obj file.
   *
   * @tparam Alloc The allocator class type to use to allocate mesh memory.
   * @param filepath The filepath to load data from.
   * @param alloc A reference to the allocator to use.
   */
  void load(const std::filesystem::path &filepath);

  /**
   * @brief Unload the memory used by the Mesh
   *
   * @tparam Del The type of the deleter class.
   * @param deleter The deleter class used to free the memory.
   */
  void unload() { std::free(mp_vertices); }

  /**
   * @brief Check if the Mesh is currently storing data.
   *
   * @return bool true if this Mesh points to usable memory.
   */
  auto loaded() { return mp_vertices != nullptr; }

  /**
   * @brief Generate gl buffers for this mesh.
   */
  void gen_buffers();

  /**
   * @brief Render primitives from array data.
   */
  void draw();

#ifndef MESH_GTEST
 private:
#endif
  unsigned int m_vbo;
  unsigned int m_vao;
  unsigned int m_ebo;
  unsigned int m_num_vertices = 0;
  Vertex *mp_vertices = nullptr;
};

#endif  // MESH_H_
