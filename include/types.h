#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

#include "lin.h"

#define HANDLE_UNUSED (UINT64_MAX)
#define HANDLE_MAX (UINT64_MAX - 1)

typedef uint64_t handle_t;
typedef uint32_t signature_t;

/**
 * @brief Stores the data required to render a mesh.
 * TODO use struct geometry instead.
 */
struct mesh {
  uint32_t m_id;
  uint32_t m_vbo;
  uint32_t m_vao;
  uint32_t m_ebo;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
  uint32_t m_tex_diff;  // TODO link this some other way.
  uint32_t m_tex_spec;  // TODO see above.
};

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

struct vertex {
  vec3 m_position;
  vec2 m_tex_coord;
  vec3 m_normal;
  vec3 m_tangent;
  vec3 m_bitangent;
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
