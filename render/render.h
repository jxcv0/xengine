#ifndef RENDER_H_
#define RENDER_H_

#include <stdint.h>

#include "light.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"

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

void render_geometries(const struct renderer *r, const mat4 projection,
                       const mat4 view, const mat4 *models,
                       const struct mesh *meshes, const uint32_t n);

void render_lighting(struct renderer *r, struct light *lights, const uint32_t n,
                     const vec3 view_pos);

#ifdef __cplusplus
}
#endif

#endif  // RENDER_H_
