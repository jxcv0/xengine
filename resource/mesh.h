#ifndef MESH_H_
#define MESH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef float vec3[3];
typedef float vec2[2];

struct vertex {
  vec3 m_position;
  vec2 m_tex_coord;
  vec3 m_normal;
};

/**
 * @brief Stored mesh data. This class is used to load and
 *        unload from a file.
 */
struct mesh {
  unsigned int m_vbo;
  unsigned int m_vao;
  size_t m_num_vertices;
  struct vertex *mp_vertices;
};

/**
 * @brief Load mesh data from an obj file.
 *
 * TODO material parsing
 *
 * @param filepath The filepath to load data from.
 * @return A mesh structure containing the data.
 */
struct mesh mesh_load(const char *filepath);

/**
 * @brief Unload the memory used by the Mesh.
 *
 * @param mesh A pointer to the mesh to unload.
 */
void mesh_unload(struct mesh *mesh);

/**
 * @brief Generate GL buffers.
 *
 * @param mesh The mesh structure.
 */
void mesh_buffer(struct mesh *mesh);

/**
 * @brief Render primitives from array data.
 *
 * @param mesh A pointer to the mesh structure.
 */
void mesh_draw(struct mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
