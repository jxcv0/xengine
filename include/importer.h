#ifndef IMPORTER_H_
#define IMPORTER_H_

#include <filesystem>

// #include "stb_image.h"
// #include "texture.h"

template <typename T>
class Importer {
 public:
  /**
   * @brief Construct an importer that will operate on a filepath.
   *
   * @param filepath The filepath of the data to import.
   */
  Importer(std::filesystem::path filepath)
      : m_filepath(filepath) {};

  ~Importer() = default;

  /**
   * @brief Import the data from the file.
   *
   * @return A pointer to the data
   */
  T* import();

 private:
  std::filesystem::path m_filepath;
};

#endif  // IMPORTER_H_
