#include "import.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "mesh.h"
#include "vec3.h"

auto xen::load_texture(const std::filesystem::path &filepath) {
  Texture texture;
  texture.mp_data = stbi_load(filepath.c_str(), &texture.m_width,
                              &texture.m_height, &texture.m_num_channels, 0);
  return texture;
}

Vec3 xen::parse_vec3(const std::string_view &sv) {
  const char *p = sv.data();
  char *end;
  float result[3];
  int i = 0;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    result[i++] = f;
    p = end;
  }
  return Vec3(result[0], result[1], result[2]);
}

Vec2 xen::parse_vec2(const std::string_view &sv) {
  const char *p = sv.data();
  char *end;
  float result[2];
  int i = 0;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    // TODO what is faster for loop with int or this?
    result[i++] = f;
    p = end;
  }
  return Vec2(result[0], result[1]);
}

Mesh::Index xen::parse_index(const std::string_view &sv) {
  using size_type = std::string_view::size_type;
  size_type curr = 0;
  size_type prev = 0;
  curr = sv.find('/', prev)) != std::string_view::npos
  auto len = curr - prev;
  auto line = sv.substr(prev, len);
  std::cout << line << " ";
  prev += len + 1;
  // do last token
  auto line = sv.substr(prev, sv.size() - prev);
  std::cout << line << "\n";

  return Mesh::Index{};
}
