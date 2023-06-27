#ifndef TYPES_H_
#define TYPES_H_

#include <stddef.h>
#include <stdint.h>

#include "glad.h"
#include "lin.h"

#define MAX_ASSET_PATH_LEN 32
#define MAX_BONE_INFLUENCE 4

enum attrib_type {
  attrib_type_MESH = 0,
  attrib_type_MATERIAL = 1,
  attrib_type_MODEL_MATRIX = 2,
  attrib_type_POSITION = 3,
  attrib_type_ROTATION = 4,
  attrib_type_MESH_LOAD_REQUEST = 5,
  attrib_type_MAT_LOAD_REQUEST = 6,
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
  vec3_t m_pos;
  vec3_t m_view_dir;
  vec3_t m_up;
  vec3_t m_right;
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
  char asset_path[MAX_ASSET_PATH_LEN];
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
  char asset_path[MAX_ASSET_PATH_LEN];
};

struct rotation {
  vec3_t axis;
  float radians;
};

struct loadreq {
  char path[MAX_ASSET_PATH_LEN];
};

typedef union attribute {
  struct mesh as_mesh;
  struct pbr_material as_material;
  vec3_t as_position;
  mat4_t as_model_matrix;
  struct loadreq as_request;
  struct rotation as_rotation;
  // ...
} attrib_t;

_Static_assert(sizeof(union attribute) == 64,
               "Component size is not equal to L1 CLS");

#endif  // TYPES_H_
