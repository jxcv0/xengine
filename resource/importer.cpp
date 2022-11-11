#include "importer.h"

#include <algorithm>
#include <stb_image.h>
#include <unistd.h>
#include <vec3.h>
#include <vec2.h>

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "mesh.h"

template <>
void import_impl::import(Texture* texture,
                         const std::filesystem::path& filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

// parse "v" and "vn" lines
std::vector<Vec3> parse_vec3_lines(const std::vector<std::string>& lines) {
  std::vector<Vec3> vectors;
  for (const auto& line : lines) {
    std::stringstream lstream(line);
    float x, y, z;
    lstream >> x >> y >> z;
    vectors.push_back(Vec3(x, y, z));
  }
  return vectors;
}

// parse "vt" lines
std::vector<Vec2> parse_vec2_lines(const std::vector<std::string>& lines) {
  std::vector<Vec2> vectors;
  for (const auto& line : lines) {
    std::stringstream lstream(line);
    float x, y;
    lstream >> x >> y;
    vectors.push_back(Vec2(x, y));
  }
  return vectors;
}

auto parse_face_tok(const std::string& face) {
  using size_type = std::string::size_type;
  unsigned int temp[3];
  size_type pos = 0, last = 0;
  for (auto i = 0; i < 3; i++) {
    pos = face.find('/', last);
    temp[i] = std::atoi(face.substr(last, pos - last).c_str());
    last = ++pos;
  }
  return Mesh::Index{--temp[0], --temp[1], --temp[2]};
}

std::vector<Mesh::Index> parse_face_lines(
    const std::vector<std::string>& lines) {
  std::vector<Mesh::Index> indices;

  for (const auto& line : lines) {
    std::stringstream lstream(line);
    std::string face;
    for (auto i = 0; i < 3; i++) {
      lstream >> face;
      indices.push_back(parse_face_tok(face));
    }
  }
  return indices;
}

template <>
void import_impl::import(Mesh* mesh, const std::filesystem::path& filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  std::vector<std::string> position_lines;
  std::vector<std::string> normal_lines;
  std::vector<std::string> tex_coord_lines;
  std::vector<std::string> face_lines;
  std::string mtllib_line;
  std::string usemtl_line;

  // divide lines up based on first token
  std::ifstream filestream(filepath);
  for (std::string line; std::getline(filestream, line);) {
    if (line.substr(0, 2) == "v ") {
      position_lines.push_back(line.substr(2));
    } else if (line.substr(0, 3) == "vn ") {
      normal_lines.push_back(line.substr(3));
    } else if (line.substr(0, 3) == "vt ") {
      tex_coord_lines.push_back(line.substr(3));
    } else if (line.substr(0, 2) == "f ") {
      face_lines.push_back(line.substr(2));
    } else if (line.substr(0, 7) == "mtllib ") {
      mtllib_line = line.substr(7);
    } else if (line.substr(0, 7) == "usemtl ") {
      usemtl_line = line.substr(7);
    }
  }

  auto positions = parse_vec3_lines(position_lines);
  auto tex_coords = parse_vec2_lines(tex_coord_lines);
  auto normals = parse_vec3_lines(normal_lines);
  auto indices = parse_face_lines(face_lines);
  mesh->m_positions = positions;
  mesh->m_tex_coords = tex_coords;
  mesh->m_normals = normals;
  mesh->m_indices = indices;
}
