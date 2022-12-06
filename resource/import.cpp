#include "import.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "mmapfile.h"

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
  (void)material;
}

template <>
void xen::import([[maybe_unused]] Mesh *mesh,
                 const std::filesystem::path &filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  MmapFile file(filepath);
  if (!file.valid()) {
  }

  using size_type = std::string_view::size_type;
  auto view = file.view();
  size_type curr = 0, prev = 0;

  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    std::cout << view.substr(prev, len);
    prev += len;
  }
}
