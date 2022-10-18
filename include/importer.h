#ifndef IMPORTER_H_
#define IMPORTER_H_

#include <filesystem>
#include <memory>

#include "stb_image.h"
#include "texture.h"

/**
 * @brief implementation of import function for Importer class.
 */
namespace import_impl {

  /**
   * @brief Unspecialized import function throws runtime error.
   */
  template <typename T> 
  void import(T* dest, const std::filesystem::path& filepath) {
    throw std::runtime_error("no implementation for importing type");
  }

  /**
   * @brief Function specialization for importing a texture.
   *
   * @param texture A pointer to the destination texture instance.
   * @param filepath The path to the file to import.
   */
  template<>
  void import(Texture *texture, const std::filesystem::path& filepath) {
    texture->mp_data =
        stbi_load(filepath.c_str(), &texture->m_width, &texture->m_height,
                  &texture->m_num_channels, 0);
  }
} // namespace importer_impl


template <typename T>
class Importer {
 public:
  /**
   * @brief Construct an importer that will operate on a filepath.
   *
   * @param filepath The filepath of the data to import.
   */
  Importer(std::filesystem::path filepath) : m_filepath(filepath){};

  ~Importer() = default;

  /**
   * @brief Import the data from the file.
   *
   * @param allocator The allocating class for the new data.
   * @return A pointer to the data.
   */
  template<typename Allocator>
  T *import(Allocator &allocator) {
    T *dest = allocator.allocate(sizeof(T)); 
    import_impl::import(dest, m_filepath);
    return dest;
  }

 private:
  std::filesystem::path m_filepath;
};


#endif  // IMPORTER_H_
