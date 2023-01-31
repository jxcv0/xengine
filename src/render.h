#ifndef RENDER_H_
#define RENDER_H_

#include <stdint.h>

#include "assets.h"
#include "light.h"
#include "lin.h"
#include "shader.h"

#define MAX_NUM_LIGHTS 128

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Buffers used by renderer.
 */
struct renderer {
  uint32_t scr_w;
  uint32_t scr_h;
  shader_t geom_shader;
  shader_t light_shader;
  uint32_t g_buff;
  uint32_t g_pos;
  uint32_t g_norm;
  uint32_t g_diffspec;
  uint32_t depth_rbo;
  uint32_t quad_vbo;
  uint32_t quad_vao;
};

/**
 * @brief Initialize a renderer r.
 *
 * @param r The renderer.
 * @param scr_w The screen width in pixels.
 * @param scr_h The screen height in pixels.
 * @return 0 on success. -1 if framebuffer is incomplete.
 */
int renderer_init(struct renderer *r, const uint32_t scr_w,
                  const uint32_t scr_h);

/**
 * @brief Render data to G-Buffer
 *
 * @param r Renderer state.
 * @param projection Projection matrix.
 * @param view View matrix.
 * @param model Model matrices.
 * @param meshes Mesh data to render.
 * @param n The number of model matrices and meshes.
 */
void render_geometries(const struct renderer *r, const mat4 projection,
                       const mat4 view, const vec3 *positions,
                       const struct mesh *meshes, const uint32_t n);

/**
 * @brief Render lighting.
 *
 * @param r Renderer state.
 * @param lights Lighting data.
 * @param n The number of lights.
 * @param view_pos The view position.
 */
void render_lighting(struct renderer *r, struct light *lights, const uint32_t n,
                     const vec3 view_pos);

#ifdef __cplusplus
}
#endif

#endif  // RENDER_H_