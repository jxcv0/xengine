#ifndef PBR_DEFERRED_H_
#define PBR_DEFERRED_H_

#include <stddef.h>
#include <stdint.h>

#include "assets.h"
#include "glad.h"
#include "light.h"
#include "lin.h"
#include "shader.h"

struct renderer {
  // geometry
  GLuint g_buff;
  GLuint g_position;

  // normal matrix
  GLuint g_tangent;
  GLuint g_bitangent;
  GLuint g_normal;

  // textures
  GLuint g_tex_diff;
  GLuint g_tex_norm;

  // depth
  GLuint g_depth;

  // lighting
  GLuint quad_vbo;
  GLuint quad_vao;

  GLuint deferred_geometry;
  GLuint deferred_lighting;
};

/**
 * @brief Initialize the renderer.
 *
 * @param scr_w The width of the screen in pixels.
 * @param scr_h The height of the screen in pixels.
 * @return On success 0, on error -1.
 */
int pbrd_init(struct renderer *r, const uint32_t scr_w, const uint32_t scr_h);

/**
 * @brief Render geometry and texture data to the G-Buffer.
 *
 * @param TODO
 */
void pbrd_render_geometries(struct renderer *r, float projection[4][4],
                            float view[4][4], float *model_matrices[4][4],
                            struct geometry *geometries,
                            struct pbr_material *materials, size_t n);
/**
 * @brief Render lighting using data stored in G-Buffer.
 * TODO
 */
void pbrd_render_lighting(struct renderer *r, struct light *lights,
                          size_t nlights, float view_pos[3], uint32_t scr_w,
                          uint32_t scr_h);

#endif  // PBR_DEFERRED_H_
