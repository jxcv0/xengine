#ifndef MESH_H_
#define MESH_H_

#include <stddef.h>
#include <stdint.h>

#include "lin.h"

#define MESH_DIR "assets/meshes/"
#define TEXTURE_DIR "assets/textures/"

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
 * @brief Stores the data required to render a mesh.
 */
struct mesh {
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
  uint32_t m_tex_diff; // TODO link this some other way.
  uint32_t m_tex_spec; // TODO see above.
};

/**
 * @brief Load a mesh by name from the mesh directory.
 *
 * @param filename The name of the file to load.
 * @return A mesh structure with OpenGL buffer object handles.
 */
struct mesh load_mesh(const char *filename);

/**
 * @brief Load a texture from the texture directory.
 *
 * @param filename The name of the file to load.
 * @return The OpenGL texture buffer.
 */
uint32_t load_texture(const char *filename);

/**
 * @brief Delete the buffers associated with a mesh.
 *
 * @param mesh A pointer to the mesh to delete.
 */
void unload_mesh(struct mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
