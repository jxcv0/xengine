#include "import.h"
#include "mmapfile.h"
#include <stdexcept>

auto load_texture(const std::filesystem::path &filepath) {
  Texture texture;
  texture.mp_data = stbi_load(filepath.c_str(), &texture.m_width,
                              &texture.m_height, &texture.m_num_channels, 0);
  return texture;
}

template <>
void xen::import(Material *material, const std::filesystem::path &filepath) {
  if (filepath.extension() != ".mtl") {
    throw std::runtime_error("file extension not supported");
  }
  (void) material;
}

template <>
void xen::import(Mesh *mesh, const std::filesystem::path &filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  MmapFile file(filepath);
  if (!file.valid()) {

  }

  file.data();
}
