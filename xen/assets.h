#ifndef MESH_H_
#define MESH_H_

#include <stddef.h>
#include <stdint.h>

#include "lin.h"

#define ASSET_MAX_NAME_LEN 64
#define TEXTURE_NAME_LEN 64
#define MAX_ASSETS 128
#define MAX_MESHES 256

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
  vec3 m_tangent;
  vec3 m_bitangent;
};

/**
 * @brief 3D geometry data that all describe the same mesh.
 */
struct geometry {
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
};

/**
 * @brief Contains ID's of diffuse, normal, roughness and displacement image
 * maps.
 */
struct pbr_material {
  uint32_t m_diffuse;
  uint32_t m_normal;
  uint32_t m_roughness;
  uint32_t m_metallic;
  // uint32_t m_displacement;
};

/**
 * @brief Stores the data required to render a mesh.
 */
struct mesh {
  uint32_t m_id;
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
  uint32_t m_tex_diff;  // TODO link this some other way.
  uint32_t m_tex_spec;  // TODO see above.
};

/**
 * @brief TODO
 */
void load_mesh(struct mesh *meshes, uint32_t *count, const char *filename);

/**
 * @brief Load a geometry from a file.
 *
 * @param filepath The path to the file to load.
 * @return A geometry containing the data in the file.
 */
struct geometry load_geometry(const char *filepath);

/**
 * @brief Load a material from a file.
 *
 * @param material_name The prefix of the textures to load.
 * @return A material containing the data in the texture group.
 */
struct pbr_material load_pbr_material(const char *material_name);

/**
 * @brief Load a texture from the texture directory.
 *
 * @param filename The name of the file to load.
 * @prarm format The GL format.
 * @return The OpenGL texture buffer.
 */
uint32_t load_texture(const char *filename);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
