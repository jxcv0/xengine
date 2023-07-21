#ifndef ASSET_TYPES_H_
#define ASSET_TYPES_H_

#define MAX_NUM_MESHES_PER_MODEL 6

#ifdef __cplusplus
extern "C" {
#endif

enum asset_type { asset_type_MESH, asset_type_TEXTURE, asset_type_TTF };

struct vertex {
  float position[3];
  float tex_coord[2];
  float normal[3];
  float tangent[3];
  float bitangent[3];
};

struct mesh {
  int model_id;
  struct vertex* vertices;
  unsigned int* indices;
  unsigned int nvertices;
  unsigned int nindices;
};

struct model {
  int meshes[MAX_NUM_MESHES_PER_MODEL];
  unsigned int nmeshes;
};

/* An image texture */
struct texture {
  union {
    int whn[3];
    struct {
      int width;
      int height;
      int nchannels;
    } sizeinfo;
  };
  unsigned char* data;
};

struct font {
  int placeholder;
};

#ifdef __cplusplus
}
#endif

#endif  // ASSET_TYPES_H_
