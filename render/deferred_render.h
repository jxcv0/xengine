#ifndef DEFERRED_RENDER_H_
#define DEFERRED_RENDER_H_

#include <stddef.h>
#include <stdint.h>

#include "lin.h"
#include "mesh.h"

/**
 * @brief Initialize the deferred renderer with a screen width and height in
 * pixels.
 *
 * @param scr_w Screen width in pixels
 * @param scr_h Screen heigh in pixels
 */
void dr_init(const uint32_t scr_w, const uint32_t scr_h);

/**
 * @brief Render geometries to the G-Buffer.
 *
 * @param projection The projection matrix
 * @param view The view matrix
 * @param model_matrices The model matrices
 * @param meshes The meshes to render.
 * @param n The number of meshes and model matrices
 */
void dr_geometry_pass(const mat4 projection, const mat4 view,
                      const mat4 *model_matrices, const struct mesh *meshes,
                      const uint32_t n, const vec3 view_pos);

#endif  // DEFERRED_RENDER_H_
