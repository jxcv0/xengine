#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#include "lin.h"

#define MAX_NUM_ENTITIES (0xFFFFFFFF)

/**
 * @brief Entity id.
 */
typedef uint32_t eid_t;

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

#endif  // TYPES_H_
