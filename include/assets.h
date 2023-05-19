#ifndef ASSETS_H_
#define ASSETS_H_

#include "glad.h"
#include "lin.h"
#include "types.h"

#define TEXTURE_DIR "assets/textures/"

/**
 * @brief Load a 3D object from a file.
 * @param geom The geometry structure to store the buffer object handles.
 * @param filepath The filepath to load the object from.
 * @return 0 on success, -1 on failure.
 */
int load_obj(struct geometry *geom, const char *filepath);

/**
 * @brief Load a PBR material from a file.
 * @details A material is a set of image textures containing  diffuse,
 * roughness, metallic and normal map.
 * @param mat The material structure to sture the buffer object handles.
 * @param material_name TODO make this filepath
 * @return 0 on success, -1 on failure.
 */
int load_pbr_material(struct pbr_material *mat, const char *material_name);

// TODO make this static
GLuint load_texture(const char *filename);

#endif  // ASSETS_H_
