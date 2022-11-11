#ifndef IMPORTER_H_
#define IMPORTER_H_

#include <material.h>
#include <model.h>
#include <shader.h>
#include <texture.h>

#include <cassert>
#include <filesystem>
#include <memory>
#include <utility>

/**
 * @brief implementation of import function for Importer class.
 */
namespace import_impl {

/**
 * @brief Unspecialized import function throws runtime error.
 */
template <typename T>
void import(T *dest, const std::filesystem::path &filepath) {
  throw std::runtime_error("no implementation for importing type");
}

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
}  // namespace import_impl

template <typename T, template <typename> typename Allocator = std::allocator>
class Importer {
 public:
  /**
   * @brief Construct an importer that will operate on a filepath.
   *
   * @param filepath The filepath of the data to import.
   */
  constexpr explicit inline Importer(const std::filesystem::path &filepath)
      : m_filepath(filepath), m_allocator() {}

  ~Importer() = default;

  /**
   * @brief Import the data from the file.
   *
   * @return A pointer to the data.
   */
  T *import() {
    T *dest = m_allocator.allocate(sizeof(T));
    import_impl::import(dest, m_filepath);
    return dest;
  }

 private:
  std::filesystem::path m_filepath;
  Allocator<T> m_allocator;
};

#endif  // IMPORTER_H_
