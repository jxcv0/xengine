#ifndef ASSETS_H_
#define ASSETS_H_

#include "glad.h"
#include "lin.h"
#include "types.h"

#define TEXTURE_DIR "assets/textures/"

int load_obj(struct geometry *geom, struct pbr_material *mat,
             const char *filepath);

int load_geometry(struct geometry *geom, const char *filepath);

int load_pbr_material(struct pbr_material *mat, const char *material_name);

GLuint load_texture(const char *filename);

#endif  // ASSETS_H_
