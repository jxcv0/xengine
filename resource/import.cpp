#include "import.h"

#include <cstddef>
#include <cstdlib>
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
  // TODO
  (void)material;
}

void parsef([[maybe_unused]] Mesh *mesh, const std::string_view sv) {
  // using size_type = std::string_view::size_type;
  // size_type curr = 0, prev = 0;

  const char *p= sv.data();
  char *end;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    std::cout << f << " ";
    p = end;
  }
}

void parsei() {
  // TODO
}

template <>
void xen::import([[maybe_unused]] Mesh *mesh,
                 const std::filesystem::path &filepath) {
  using size_type = std::string_view::size_type;
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  const MmapFile file(filepath);
  auto view = file.view();
  size_type curr = 0, prev = 0;

  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;

    if ("v " == line.substr(0, 2)) {
      parsef(mesh, line.substr(2));
      break;
    } else if ("vt " == line.substr(0, 3)) {

    }
  }
}
