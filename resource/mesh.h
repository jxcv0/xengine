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
  const static int id = (1 << 0);
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
  void unload() { delete[] mp_vertices; }

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
  struct Index {
    unsigned int m_position_idx;
    unsigned int m_tex_coord_idx;
    unsigned int m_normal_idx;

    constexpr inline auto operator==(const Index &other) const {
      return other.m_position_idx == m_position_idx &&
             other.m_normal_idx == m_normal_idx &&
             other.m_tex_coord_idx == m_tex_coord_idx;
    }
  };

  Vec3 parse_vec3(const std::string_view &sv);
  Vec2 parse_vec2(const std::string_view &sv);
  Index parse_index(const std::string_view &sv);

  unsigned int m_vbo;
  unsigned int m_vao;
  // unsigned int m_ebo; // TODO worth it?
  unsigned int m_num_vertices = 0;
  Vertex *mp_vertices = nullptr;
};

#endif  // MESH_H_
