#ifndef RENDER_H_
#define RENDER_H_

#include "mesh.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#endif  // RENDER_H_
