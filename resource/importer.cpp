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

static inline auto parse_mtllib(std::filesystem::path filepath,
                                std::stringstream& line) {
  std::string str;
  line >> str;

  filepath.replace_filename(str);
  std::ifstream mtllib_stream(filepath);
  // TODO parse .mtl file
  return nullptr;
}

template<>
void import_impl::import(Mesh *mesh, const std::filesystem::path& filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }
  std::ifstream filestream(filepath);
  for(std::string line; std::getline(filestream, line); ) {
    std::stringstream linestream(line);
    std::string line_token;
    linestream >> line_token;
    if (line_token == "#") {
      // skip comments
      continue;
    } else if (line_token == "mtllib") {
      auto mtllib = parse_mtllib(filepath, linestream);
    } else if (line_token == "o") {
      std::cout << "object name\n";
    } else if (line_token == "v") {
      std::cout << "vertex\n";
    } else if (line_token == "vt") {
      std::cout << "tex coord\n";
    } else if (line_token == "vn") {
      std::cout << "normal\n";
    } else if (line_token == "usemtl") {
      std::cout << "material\n";
    } else if (line_token == "s") {
      std::cout << "smoothing\n";
    } else if (line_token == "f") {
      std::cout << "face\n";
    }
  }
}
