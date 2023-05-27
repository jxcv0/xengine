#ifndef TYPES_H_
#define TYPES_H_

#include <stddef.h>
#include <stdint.h>

#include "glad.h"
#include "lin.h"

#define MAX_ASSET_PATH_LEN 32

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
 *
 * NOTE: the order of the members is important see assets.c:load_mtl
 */
struct pbr_material {
  union {
    GLuint arr[4];
    struct {
      GLuint diffuse;
      GLuint normal;
      GLuint roughness;
      GLuint metallic;
    };
  };
};

struct vertex {
  // uint32_t bone_ids[MAX_BONE_INFLUENCE];
  float position[3];
  float tex_coord[2];
  float normal[3];
  float tangent[3];
  float bitangent[3];
};

/**
 * @brief 3D geometry data
 */
struct mesh {
  GLuint vbo;
  GLuint vao;
  GLuint ebo;
  size_t num_vertices;
  size_t num_indices;
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
  char path[MAX_ASSET_PATH_LEN];
};

#endif  // TYPES_H_
