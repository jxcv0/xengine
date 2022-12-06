#ifndef IMPORT_H_
#define IMPORT_H_

#include <filesystem>

#include "material.h"
#include "mesh.h"

namespace xen {

/**
 * @brief Unspecialized import function throws runtime error.
 */
template <typename T>
void import([[maybe_unused]] T *dest,
            [[maybe_unused]] const std::filesystem::path &filepath) {
  throw std::runtime_error("no implementation for importing type");
}

// TODO mesh and material to be treated as separate components??

/**
 * @brief Function specialization for importing a mesh from a wavefront .obj
 *        file. Meshes MUST be triangulated.
 *
 * @param mesh A pointer to the destination mesh.
 * @param filepath The path to the file to import.
 */
template <>
void import(Mesh *mesh, const std::filesystem::path &filepath);

/**
 * @brief Function specialization for importing a material from a wavefront
 *        .mtl file.
 *
 * @param texture A pointer to the destination material.
 * @param filepath The path to the file to import.
 */
template <>
void import(Material *material, const std::filesystem::path &filepath);
}  // namespace xen

#endif  // IMPORT_H_
