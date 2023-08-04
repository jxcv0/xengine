#ifndef ASSETMGR_H_
#define ASSETMGR_H_

#include <semaphore.h>
#include <stddef.h>

#include "asset_types.h"

#define MAX_NUM_MESHES 64

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*allocator)(size_t);

/*
struct asset
{
  union
  {
    struct mesh as_mesh;
    struct texture as_texture;
  } data;
  unsigned int refcount;
  char filepath[32];
};
*/

struct mesh_allocator {
  sem_t sem;
  struct mesh *buf;
  int *free;
  const unsigned int bufsize;
  unsigned int nmeshes;
  unsigned int nfree;
};

struct mesh_allocator create_mesh_allocator(int bufsize);

/* allocate a mesh and get the index */
int alloc_mesh(struct mesh_allocator *alloc);

/* Free a mesh at index */
int free_mesh(struct mesh_allocator *alloc, int index);

/**
 * @brief Get the type of an asset based on a file extension.
 *
 * @param filepath The filepath to extract the asset type from
 * @return The type of the asset with the filepath.
 */
int asset_type(const char *filepath);

struct texture load_texture(const char *filepath);

struct model load_model(int id, const char *filepath, struct mesh *mesh_buf,
                        unsigned int *nmeshes);

struct font *load_font(struct font *font, const char *filepath,
                       allocator alloc_font);

#ifdef __cplusplus
}
#endif

#endif // ASSETMGR_H_
