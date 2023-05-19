#ifndef TYPES_H_
#define TYPES_H_

#include <stddef.h>
#include <stdint.h>

#include "glad.h"
#include "lin.h"

#define MAX_ASSET_PATH_LEN 32

/**
 * @brief Stores the data required to render a mesh.
 * TODO use struct geometry instead.
 */
struct mesh {
  GLuint id;
  GLuint vbo;
  GLuint vao;
  GLuint ebo;
  GLuint num_vertices;
  GLuint num_indices;
  GLuint tex_diff;  // TODO link this some other way.
  GLuint tex_spec;  // TODO see above.
};

/**
 * @brief data for calculating view matrix.
 */
struct camera {
  float m_yaw;
  float m_pitch;
  float m_movement_speed;
  float m_mouse_sensetivity;
  float m_pos[3];
  float m_view_dir[3];
  float m_up[3];
  float m_right[3];
};

/**
 * @brief Contains ID's of diffuse, normal, roughness and metallic image
 * maps.
 */
struct pbr_material {
  GLuint diffuse;
  GLuint normal;
  GLuint roughness;
  GLuint metallic;
  // GLuint displacement;
};

struct vertex {
  float position[3];
  float tex_coord[2];
  float normal[3];
  float tangent[3];
  float bitangent[3];
};

/**
 * @brief 3D geometry data
 */
struct geometry {
  GLuint vbo;
  GLuint vao;
  GLuint ebo;
  size_t num_vertices;
  size_t num_indices;

  // TODO separate this out and make rendering a system.
  struct pbr_material material;
};

struct model_matrix {
  float elem[4][4];
};

struct position {
  union {
    float vec[3];
    struct {
      float x;
      float y;
      float z;
    };
  };
};

struct loadreq {
  char mesh_path[MAX_ASSET_PATH_LEN];
  char mtl_math[MAX_ASSET_PATH_LEN];
};

#endif  // TYPES_H_
