#ifndef ASSETS_H_
#define ASSETS_H_

#include "glad.h"
#include "lin.h"
#include "types.h"

#define TEXTURE_DIR "assets/textures/"

/**
 * @brief Load a 3D object from a xengine binary file.
 * @param geom The geometry structure to store the buffer object handles.
 * @param filepath The filepath to load the object from.
 * @return 0 on success, -1 on failure.
 */
int load_obj(struct geometry *geom, const char *filepath);

/**
 * @brief Load a material from a xengine binary file.
 * @param mat The material structure to store the image buffer handles.
 * @param filepath The file to load the material from.
 * @return 0 on success, -1 on failure.
 */
int load_mtl(struct pbr_material *mat, const char *filepath);

#endif  // ASSETS_H_
