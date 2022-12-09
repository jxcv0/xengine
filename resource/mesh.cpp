#include "mesh.h"

Vec3 Mesh::parse_vec3(const std::string_view &sv) {
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

Vec2 Mesh::parse_vec2(const std::string_view &sv) {
  const char *p = sv.data();
  char *end;
  float result[2];
  int i = 0;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    result[i++] = f;
    p = end;
  }
  return Vec2(result[0], result[1]);
}

Mesh::Index Mesh::parse_index(const std::string_view &sv) {
  using size_type = std::string_view::size_type;
  size_type curr = 0;
  size_type prev = 0;

  Mesh::Index index;
  curr = sv.find('/', prev);
  auto len = curr - prev;
  auto pos_index = sv.substr(prev, len);
  index.m_position_idx = std::atoi(pos_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', prev);
  len = curr - prev;
  auto tex_coords_index = sv.substr(prev, len);
  index.m_tex_coord_idx = std::atoi(tex_coords_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', sv.size() - prev);
  len = curr - prev;
  auto normal_index = sv.substr(prev, len);
  index.m_normal_idx = std::atoi(normal_index.data()) - 1;

  return index;
}
