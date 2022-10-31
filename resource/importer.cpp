#include "importer.h"

#include <fstream>

template <>
void import_impl::import(Texture *texture,
                         const std::filesystem::path &filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

auto import_vertex_shader(auto filepath) {
  std::ifstream in(filepath);
  // TODO 
}

auto import_fragment_shader(auto filepath) {
  std::ifstream in(filepath);
  // TODO 
}

template <>
void import_impl::import(Shader *shader, const std::filesystem::path &filepath) {
  if (filepath.extension() == ".vert") {
    import_vertex_shader(filepath);
  } else if (filepath.extension() == ".frag") {
    import_fragment_shader(filepath);
  }
}
