#ifndef ASSETMGR_H_
#define ASSETMGR_H_

#include "asset_types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*allocator) (size_t);

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

/**
 * @brief Get the type of an asset based on a file extension.
 *
 * @param filepath The filepath to extract the asset type from
 * @return The type of the asset with the filepath.
 */
int asset_type (const char *filepath);

struct texture *load_texture (const char *filepath, allocator alloc_tex);

struct model load_meshes (const char *filepath, allocator alloc_mesh);

struct font *load_font (struct font *font, const char *filepath,
                        allocator alloc_font);

#ifdef __cplusplus
}
#endif

#endif // ASSETMGR_H_
