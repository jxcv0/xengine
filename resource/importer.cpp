#include "importer.h"
#include "vec2.h"

#include <ios>
#include <stb_image.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
#include <vec3.h>

template <>
void import_impl::import(Texture *texture,
                         const std::filesystem::path &filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

// parse "v" and "vn" lines
std::vector<Vec3> parse_vec3_lines(const std::vector<std::string>& lines) {
  std::vector<Vec3> vectors;
  for (auto& line : lines) {
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
  for (auto& line : lines) {
    std::stringstream lstream(line);
    float x, y;
    lstream >> x >> y;
    vectors.push_back(Vec2(x, y));
  }
  return vectors;
}

// a set of indexes for uncompressing .obj
struct IndexSet {
  int position[3];
  int normal[3];
  int tex_coords[3];
};

// separate "f" lines into individual index strings
std::vector<std::string> separate_face_line(const std::string& line) {
  std::vector<std::string> strings(3);
  std::stringstream lstream(line);
  std::string s1, s2, s3;
  lstream >> s1 >> s2 >> s2;
  strings.push_back(s1);
  strings.push_back(s2);
  strings.push_back(s3);
  return strings;
}

template <>
void import_impl::import(Mesh *mesh, const std::filesystem::path &filepath) {
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
  auto normals = parse_vec3_lines(normal_lines);
  auto tex_coords = parse_vec2_lines(tex_coord_lines);

  for (auto pos : positions) {
      std::cout << pos << "\n";
  }
}
