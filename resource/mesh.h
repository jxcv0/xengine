#ifndef MESH_H_
#define MESH_H_

#include "vec2.h"
#include "vec3.h"

/**
 * @brief Stored mesh data. This class is used to load and
 *        unload from a file.
 */
struct Mesh {
  /**
   * @brief Component id
   */
  const static int component_id = (1 << 0);

  struct Vertex {
    Vec3 m_position;
    Vec3 m_normal;
    Vec2 m_tex_coord;
  };

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

  // TODO Move these to other component. along with draw func
  unsigned int m_vbo;
  unsigned int m_vao;
  unsigned int m_num_vertices = 0;
  Vertex *mp_vertices = nullptr;
};

/**
 * @brief Load mesh data from an obj file.
 *
 * @param filepath The filepath to load data from.
 */
Mesh load_mesh(const char *filepath);

/**
 * @brief Unload the memory used by the Mesh.
 */
void unload_mesh(Mesh *mesh) {
  delete[] mesh->mp_vertices;
  mesh->mp_vertices = nullptr;
}

/**
 * @brief Generate gl buffers for this mesh.
 */
void gen_mesh_buffers(Mesh *mesh);

/**
 * @brief Render primitives from array data.
 */
void draw_mesh(Mesh *mesh);

#endif  // MESH_H_
