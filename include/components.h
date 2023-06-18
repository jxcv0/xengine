#ifndef TYPES_H_
#define TYPES_H_

#include <stddef.h>
#include <stdint.h>

#include "glad.h"
#include "lin.h"

#define MAX_ASSET_PATH_LEN 32
#define MAX_BONE_INFLUENCE 4

enum component_type {
  MESH = 0,
  MATERIAL = 1,
  MODEL_MATRIX = 2,
  POSITION = 3,
  ROTATION = 4,
  MESH_LOAD_REQUEST = 5,
  MAT_LOAD_REQUEST = 6,
  // ANIM_LOAD_REQUEST 6
  // AUDIO_LOAD_REQUEST 7
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
  float position[3];
  float tex_coord[2];
  float normal[3];
  float tangent[3];
  float bitangent[3];
  uint32_t bone_ids[MAX_BONE_INFLUENCE];
  float bone_weights[MAX_BONE_INFLUENCE];
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
    float elem[3];
    struct {
      float x;
      float y;
      float z;
    };
  };
};

struct rotation {
  float axis[3];
  float radians;
};

struct loadreq {
  char path[MAX_ASSET_PATH_LEN];
};

union component {
  struct mesh as_mesh;
  struct pbr_material as_material;
  struct position as_position;
  struct model_matrix as_model_matrix;
  struct loadreq as_request;
  struct rotation as_rotation;
  struct spin as_spin;
  // ...
};

_Static_assert(sizeof(union component) == 64,
               "union component size exceeds L1 CLS");

#endif  // TYPES_H_
