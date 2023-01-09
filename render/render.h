#ifndef RENDER_H_
#define RENDER_H_

#include "light.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate GL buffers.
 *
 * @param mesh The mesh structure.
 */
void gen_mesh_buffers(struct mesh *mesh);

/**
 * @brief Generate GL texture buffers.
 *
 * @param material A pointer to the material structure.
 */
void gen_material_buffers(struct material *material);

/**
 * @brief Render primitives from array data.
 *
 * @param mesh A pointer to the mesh structure.
 * @param TODO
 */
void draw_mesh(const shader_t shader, const mat4 projection_matrix,
               const mat4 view_matrix, const mat4 model_matrix,
               const vec3 view_position, struct light *light,
               const struct mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // RENDER_H_