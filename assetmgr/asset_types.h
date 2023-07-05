#ifndef ASSET_TYPES_H_
#define ASSET_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

struct mesh
{
  float *vertices;
  unsigned int *indices;
  unsigned int nvertices;
  unsigned int nindices;
};

struct texture
{
  unsigned char *img;
};

#ifdef __cplusplus
}
#endif

#endif // ASSET_TYPES_H_