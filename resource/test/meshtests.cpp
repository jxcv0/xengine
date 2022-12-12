#include <gtest/gtest.h>

#include <string_view>

#include "vec3.h"

#define MESH_GTEST
#include "mesh.h"

TEST(meshtests, parse_vec3) {
  Mesh m;
  std::string_view str{"0.22 -0.42 0.001"};
  auto vec = m.parse_vec3(str);
  ASSERT_EQ(Vec3(0.22, -0.42, 0.001), vec);
}

TEST(meshtests, parse_vec2) {
  Mesh m;
  std::string_view str{"-0.42 0.001"};
  auto vec = m.parse_vec2(str);
  ASSERT_EQ(Vec2(-0.42, 0.001), vec);
}

TEST(meshtests, parse_index) {
  Mesh m;
  std::string_view str{"1/2/3"};
  auto index = m.parse_index(str);
  Mesh::Index exp;
  exp.m_position_idx = 0;
  exp.m_tex_coord_idx = 1;
  exp.m_normal_idx = 2;
  ASSERT_EQ(exp, index);
}

TEST(meshtests, mesh) {
  Mesh mesh;
  mesh.load("assets/models/cube/cube.obj");

  ASSERT_EQ(mesh.m_num_vertices, 36);

  ASSERT_EQ(mesh.mp_vertices[0].m_position, Vec3(1, -1, 1));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_position, Vec3(-1, 1, -1));

  ASSERT_EQ(mesh.mp_vertices[0].m_normal, Vec3(0, -1, 0));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_normal, Vec3(0, 0, -1));

  ASSERT_EQ(mesh.mp_vertices[0].m_tex_coord, Vec2(1, 0.333333));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_tex_coord,
            Vec2(0, 0.666667));

  mesh.unload();
}
