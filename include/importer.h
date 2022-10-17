#ifndef IMPORTER_H_
#define IMPORTER_H_

#include <filesystem>
#include <memory>

#include "stb_image.h"
#include "texture.h"

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
   * @return A pointer to the data
   */
  template <typename Allocator>
  T* import(Allocator allocator);

 private:
  std::filesystem::path m_filepath;
};

// template<>
//
//   Texture *texture;
//   texture->mp_data = stbi_load(m_filepath.c_str(), &texture->m_width,
//                               &texture->m_height, &texture->m_num_channels,
//                               0);
//   return texture;

#endif  // IMPORTER_H_
