#include "importer.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <stdexcept>
#include <string>
#include <vector>

template <>
void import_impl::import(Texture *texture,
                         const std::filesystem::path &filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

template<>
void import_impl::import(Model *model, const std::filesystem::path& filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }
  std::ifstream filestream(filepath);
  for(std::string line; std::getline(filestream, line); ) {
    std::stringstream linestream(line);
    std::string line_token;
    linestream >> line_token;
    if (line_token == "v") {
      // ...
    } else if (line_token == "vt") {
      // ...
    }
    // and so on ...
  }
}
