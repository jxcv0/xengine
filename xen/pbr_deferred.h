#ifndef PBR_DEFERRED_H_
#define PBR_DEFERRED_H_

#include <stdint.h>

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
 * @param
 */
void pbrd_render_geometries();

/**
 * @brief Render lighting using data stored in G-Buffer.
 */
void pbrd_render_lighting();

#ifdef __cplusplus
}
#endif

#endif  // PBR_DEFERRED_H_
