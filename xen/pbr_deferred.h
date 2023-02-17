#ifndef PBR_DEFERRED_H_
#define PBR_DEFERRED_H_

#include <stdint.h>

#include "assets.h"
#include "light.h"
#include "lin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the renderer.
 *
 * @param scr_w The width of the screen in pixels.
 * @param scr_h The height of the screen in pixels.
 * @return On success 0, on error -1.
 */
int pbrd_init(const uint32_t scr_w, const uint32_t scr_h);

/**
 * @brief Render geometry and texture data to the G-Buffer.
 *
 * @param TODO
 */
void pbrd_render_geometries(const mat4 projection, const mat4 view,
                            const mat4 *model_matrices,
                            const struct geometry *geometries,
                            const struct pbr_material *materials,
                            const uint32_t n);
/**
 * @brief Render lighting using data stored in G-Buffer.
 * TODO
 */
void pbrd_render_lighting(struct light *lights, const uint32_t n,
                          const vec3 view_pos, const uint32_t scr_w,
                          const uint32_t scr_h);

#ifdef __cplusplus
}
#endif

#endif  // PBR_DEFERRED_H_
