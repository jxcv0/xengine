#include "import.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

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

void xen::parse_index(Mesh::Index *index, const std::string_view &sv) {
  std::string_view::size_type curr = 0;
  std::string_view::size_type prev = 0;
  int i = 0;
  while ((curr = sv.find(' ', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto face_tok = sv.substr(prev, len);
    std::string_view::size_type c = 0;
    std::string_view::size_type p = 0;
    unsigned int result[3];
    int j = 0;
    while ((c = face_tok.find('/', p)) != std::string_view::npos) {
      auto l = c - p;
      result[j++] = std::atoi(face_tok.substr(p, l).data());
      p += l + 1;
    }
    index[i++] = {--result[0], --result[1], --result[2]};
    prev += len;
  }
}
