#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#include "lin.h"

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: using "m_" to prefix members implies that the member is not intented
// to be changed publicly. This also goes for all structures defined
// elsewhere.

/**
 * @brief data for calculating view matrix.
 */
struct camera {
  float m_yaw;
  float m_pitch;
  float m_movement_speed;
  float m_mouse_sensetivity;
  vec3 m_pos;
  vec3 m_view_dir;
  vec3 m_up;
  vec3 m_right;
};

/**
 * @brief Contains ID's of diffuse, normal, roughness and metallic image
 * maps.
 */
struct pbr_material {
  uint32_t m_diffuse;
  uint32_t m_normal;
  uint32_t m_roughness;
  uint32_t m_metallic;
  // uint32_t m_displacement;
};

/**
 * @brief 3D geometry data
 */
struct geometry {
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;

  // TODO separate this out and make rendering a system.
  struct pbr_material m_material;
};

#ifdef __cplusplus
}
#endif

#endif  // TYPES_H_
