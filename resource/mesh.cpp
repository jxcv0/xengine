#include "mesh.h"

#include <cstdlib>

#include "mmapfile.h"
#include "vec2.h"
#include "vec3.h"

struct Index {
  unsigned int m_position_idx;
  unsigned int m_tex_coord_idx;
  unsigned int m_normal_idx;

  constexpr inline auto operator==(const Index &other) const {
    return other.m_position_idx == m_position_idx &&
           other.m_normal_idx == m_normal_idx &&
           other.m_tex_coord_idx == m_tex_coord_idx;
  }
};

Vec3 parse_vec3(const std::string_view &sv) {
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

Vec2 parse_vec2(const std::string_view &sv) {
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

Index parse_index(const std::string_view &sv) {
  using size_type = std::string_view::size_type;
  size_type curr = 0;
  size_type prev = 0;

  Index index;
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

void Mesh::load(const std::filesystem::path &filepath) {
  using size_type = std::string_view::size_type;

  const MmapFile file(filepath);
  auto view = file.view();

  // count up how much memory to allocate.
  unsigned int temp_num_positions = 0;
  unsigned int temp_num_tex_coords = 0;
  unsigned int temp_num_normals = 0;
  unsigned int temp_num_indices = 0;
  size_type curr = 0;
  size_type prev = 0;
  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;
    if ("v " == line.substr(0, 2)) {
      temp_num_positions++;
    } else if ("vt " == line.substr(0, 3)) {
      temp_num_tex_coords++;
    } else if ("vn " == line.substr(0, 3)) {
      temp_num_normals++;
    } else if ("f " == line.substr(0, 2)) {
      temp_num_indices++;
    }
  }

  Vec3 temp_positions[temp_num_positions];
  Vec3 temp_normals[temp_num_normals];
  Vec2 temp_tex_coords[temp_num_tex_coords];
  Index temp_indices[temp_num_indices];
  temp_num_positions = 0;
  temp_num_tex_coords = 0;
  temp_num_normals = 0;
  temp_num_indices = 0;

  curr = 0;
  prev = 0;
  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;

    if ("v " == line.substr(0, 2)) {
      temp_positions[temp_num_positions++] = parse_vec3(line.substr(2));
    } else if ("vt " == line.substr(0, 3)) {
      temp_tex_coords[temp_num_tex_coords++] = parse_vec2(line.substr(3));
    } else if ("vn " == line.substr(0, 3)) {
      temp_normals[temp_num_normals++] = parse_vec3(line.substr(3));
    } else if ("f " == line.substr(0, 2)) {
      auto f_line = line.substr(2);
      size_type f_curr = 0;
      size_type f_prev = 0;
      // delim by spaces
      while ((f_curr = f_line.find(" ", f_prev)) != std::string_view::npos) {
        auto f_len = f_curr - f_prev;
        auto face_tok = f_line.substr(f_prev, f_len);
        temp_indices[temp_num_indices++] = parse_index(face_tok);
        f_prev += f_len + 1;
      }
      // do last token
      temp_indices[temp_num_indices++] =
          parse_index(f_line.substr(f_prev, f_line.size() - f_prev - 1));
    }
    // TODO mtl parsing here
  }

  // TODO a fast way of detecting duplicate indices for ebo
  mp_vertices =
      static_cast<Vertex *>(std::malloc(sizeof(Vertex) * temp_num_indices));

  for (unsigned int i = 0; i < temp_num_indices; i++) {
    auto &v = mp_vertices[i];
    auto index = temp_indices[i];
    v.m_position = temp_positions[index.m_position_idx];
    v.m_normal = temp_normals[index.m_normal_idx];
    v.m_tex_coord = temp_tex_coords[index.m_tex_coord_idx];
  }

  m_num_vertices = 0;

  /**
  auto positions_size = num_positions * sizeof(Vec3);
  auto tex_coords_size = num_tex_coords * sizeof(Vec2);
  auto normals_size = num_normals * sizeof(Vec3);
  auto faces_size = num_faces * sizeof(Mesh::Index) * 3;  // 3 per line
  auto mem_size = positions_size + tex_coords_size + normals_size + faces_size;

  // TODO need an allocator here.
  // pointers into memory block
  auto *mp_memory_block = std::malloc(mem_size);
  auto addr = reinterpret_cast<std::uintptr_t>(mp_memory_block);

  mp_positions = reinterpret_cast<Vec3 *>(addr);
  mp_normals = reinterpret_cast<Vec3 *>(addr + positions_size);
  mp_tex_coords =
      reinterpret_cast<Vec2 *>(addr + positions_size + normals_size);
  mp_indices = reinterpret_cast<Mesh::Index *>(addr + positions_size +
                                               normals_size + tex_coords_size);

  // the actual parsing
  */
}

void Mesh::gen_buffers() {
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);

  auto stride = 8 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void Mesh::draw() {
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT,
                 static_cast<void *>(0));
}
