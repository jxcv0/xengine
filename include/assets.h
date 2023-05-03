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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct geometry *(*alloc_geometries)(size_t);
typedef struct pbr_material *(*alloc_pbr_materials)(size_t);

/**
 * @brief TODO
 */
void load_mesh(struct mesh *meshes, uint32_t *count, const char *filename);

/**
 * @brief Load a geometry from a file.
 *
 * @param filepath The path to the file to load.
 * @return A geometry containing the data in the file.
 */
struct geometry load_geometry(const char *filepath);

/**
 * @brief Load a material from a file.
 *
 * @param material_name The prefix of the textures to load.
 * @return A material containing the data in the texture group.
 */
struct pbr_material load_pbr_material(const char *material_name);

/**
 * @brief Load a texture from the texture directory.
 *
 * @param filename The name of the file to load.
 * @prarm format The GL format.
 * @return The OpenGL texture buffer.
 */
uint32_t load_texture(const char *filename);

/**
 * @brief Load a collection of meshes and textures from a file.
 *
 * @param filepath The path to the .model file
 * @param alloc_geom Geometry allocation function.
 */
void load_model(const char *filepath, alloc_geometries alloc_geom);

#ifdef __cplusplus
}
#endif

#endif  // MESH_H_
