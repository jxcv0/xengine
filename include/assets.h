#ifndef MESH_H_
#define MESH_H_

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
 * @brief Get the next available geometry from a geometry array. This function
 * returns an index into a lookup table so that the geometries in the array can
 * be moved around while still being accessable with the same index.
 *
 * @param arr The array to fetch the geometry from.
 * @param table The handle to array index lookup table.
 * @param size The size of the array pointed to by arr.
 * @param count The current number of elements in use in arr.
 * @return A handle (index) that can be used to access the geometry in arr. Or
 * -1 on error.
 */
int32_t provision_geometry(struct geometry *arr, size_t *table,
                           const size_t size, size_t *count);

/**
 * @brief Make a geometry available for reassignment and unmap the indexes in
 * the index table. This is implemented by swapping the geometry pointed to by
 * handle with the last geometry, updating the table with the new indexes then
 * decrementing count.
 *
 * @param arr The array to fetch the geometry from.
 * @param table The handle to array index lookup table.
 * @param size The size of the array pointed to by arr.
 * @param count The current number of elements in use in arr.
 * @param handle The geometry handle to delete.
 * @return 0 on success. -1 on error.
 */
int32_t release_geometry(struct geometry *arr, size_t *table, const size_t size,
                         size_t *count, int32_t handle);

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
