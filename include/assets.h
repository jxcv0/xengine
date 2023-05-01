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

typedef uint64_t handle_t;

/**
 * @brief Maps a handle to an index.
 */
struct handle_index_pair {
  handle_t handle;
  size_t index;
};

/**
 * TODO this might be problematic
 *
 * @brief Generate a new unique asset handle
 */
handle_t new_asset_handle(void);

/**
 * @brief Allocate a new geometry.
 *
 * @param arr The array to provision from.
 * @param table The table to store handle to index mapping.
 * @param size The number of elements in the arrays arr and table.
 * @param count A pointer to the variable used to store the number of elements
 * in arr currently in use.
 * @return 0 on success, -1 on error.
 */
int provision_geometry(struct geometry *arr, struct handle_index_pair *table,
                       size_t size, size_t *count, handle_t handle);

/**
 * @brief Access a geometry in a geometry array.
 *
 * @param arr The array in which a geometry is stored.
 * @param table The handle to index table for the array
 */
struct geometry *get_geometry(struct geometry *arr,
                              struct handle_index_pair *table,
                              const size_t size, handle_t handle);
/**
 * @brief Make a geometry available for reassignment and unmap the indexes in
 * the index table.
 *
 * @detail This is implemented by swapping the geometry pointed to by
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
