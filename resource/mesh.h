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
  // vec3 m_tangent;
  // vec3 m_bitangent;
};

struct texture {
  unsigned int m_texture_id;
  int m_width;
  int m_height;
  int m_num_channels;
  unsigned char *mp_data;
};

/**
 * @brief Material data.
 *
 * TODO transparency d/Tr, opacity and emissiveness also?
 *
 */
struct material {
  float m_specular_exponent;      // Ns
  vec3 m_ambient_color;           // Ka
  vec3 m_diffuse_color;           // Kd
  vec3 m_specular_color;          // Ks
  struct texture m_tex_diffuse;   // map_kd
  struct texture m_tex_normal;    // map_Bump / bump
  struct texture m_tex_specular;  // map_Ks
};

/**
 * @brief Mesh data. This class is used to load and
 *        unload from a file.
 */
struct mesh {
  unsigned int m_vbo;
  unsigned int m_vao;
  uint32_t m_num_vertices;
  size_t m_num_indices;
  struct material m_material;
  struct vertex *mp_vertices;
  uint32_t *mp_indices;
};

/**
 * @brief Load mesh data from an obj file.
 *
 * TODO material parsing
 *
 * @param filepath The filepath to load data from.
 * @return A mesh structure containing the data.
 */
struct mesh load_mesh(const char *filepath);

/**
 * @brief Load material data from an obj file. Only supports one material
 * defenition
 *
 * @param obj_filepath.
 * @return The material structure.
 */
struct material load_material(const char *filepath);

/**
 * @brief Load a texture into memory.
 *
 * @brief filepath The filepath to load from.
 * @brief The texture structure.
 */
struct texture load_texture(const char *obj_filepath,
                            const char *texture_filename, size_t len);

/**
 * @brief Unload the memory used by the Mesh.
 *
 * @param mesh A pointer to the mesh to unload.
 */
void unload_mesh(struct mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
