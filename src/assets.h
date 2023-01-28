#ifndef MESH_H_
#define MESH_H_

#include <stddef.h>
#include <stdint.h>

#include "lin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Vertex position and texture data.
 */
struct vertex {
  vec3 m_position;
  vec2 m_tex_coord;
  vec3 m_normal;
};

/**
 * @brief Mesh data. This class is used to load and
 *        unload from a file.
 */
struct mesh {
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
  uint32_t m_tex_diff;
  uint32_t m_tex_spec;
};

/**
 * @brief TODO
 */
struct mesh load_mesh(const char *filepath);

/**
 * @brief TODO
 */
uint32_t load_texture(const char *filename);

/**
 * @brief TODO
 */
void unload_mesh(struct mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
