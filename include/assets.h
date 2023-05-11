#ifndef MESH_H_
#define MESH_H_

/**
 * @file assets.h
 * Loading assets from file.
 */

#include <stddef.h>

#include "lin.h"
#include "types.h"

#define MESH_DIR "assets/meshes/"
#define TEXTURE_DIR "assets/textures/"

typedef struct geometry *(*alloc_geometries)(size_t);
typedef struct pbr_material *(*alloc_pbr_materials)(size_t);

int load_geometry(struct geometry *geom, const char *filepath);

int load_pbr_material(struct pbr_material *mat, const char *material_name);

GLuint load_texture(const char *filename);

#endif  // MESH_H_
