#ifndef TYPES_H_
#define TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include "glad.h"

#include "lin.h"

#define MAX_ASSET_PATH_LEN 60

/**
 * @brief Stores the data required to render a mesh.
 * TODO use struct geometry instead.
 */
struct mesh {
  GLuint m_id;
  GLuint m_vbo;
  GLuint m_vao;
  GLuint m_ebo;
  GLuint m_num_vertices;
  GLuint m_num_indices;
  GLuint m_tex_diff;  // TODO link this some other way.
  GLuint m_tex_spec;  // TODO see above.
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
  GLuint m_diffuse;
  GLuint m_normal;
  GLuint m_roughness;
  GLuint m_metallic;
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
  GLuint m_vbo;
  GLuint m_vao;
  GLuint m_ebo;
  size_t m_num_vertices;
  size_t m_num_indices;

  // TODO separate this out and make rendering a system.
  struct pbr_material m_material;
};

struct position {
  union {
    vec3 vec;
    struct {
      float x;
      float y;
      float z;
    };
  };
};

enum asset_type {
  GEOMETRY_ASSET,
  MATERIAL_ASSET
};

struct asset {
  enum asset_type type;
  char path[MAX_ASSET_PATH_LEN];
};

#endif  // TYPES_H_
