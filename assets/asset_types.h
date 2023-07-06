#ifndef ASSET_TYPES_H_
#define ASSET_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

enum asset_type
{
  asset_type_MESH,
  asset_type_TEXTURE
};

/* */
struct mesh
{
  float *vertices;
  unsigned int *indices;
  unsigned int nvertices;
  unsigned int nindices;
};

/* An image texture */
struct texture
{
  union
  {
    int whn[3];
    struct
    {
      int width;
      int height;
      int nchannels;
    } sizeinfo;
  };
  unsigned char *data;
};

#ifdef __cplusplus
}
#endif

#endif // ASSET_TYPES_H_
