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
  GLuint geom_shader;
  GLuint light_shader;
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
int renderer_init(struct renderer *r, uint32_t scr_w, uint32_t scr_h);

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
void render_geometries(struct renderer *r, mat4 projection, mat4 view,
                       vec3 *positions, struct mesh *meshes, uint32_t n);

/**
 * @brief Render lighting.
 *
 * @param r Renderer state.
 * @param lights Lighting data.
 * @param n The number of lights.
 * @param view_pos The view position.
 */
void render_lighting(struct renderer *r, struct light *lights, uint32_t n,
                     vec3 view_pos);

#ifdef __cplusplus
}
#endif

#endif  // RENDER_H_
