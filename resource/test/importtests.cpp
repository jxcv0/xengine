#include "vec3.h"
#include <gtest/gtest.h>

#define MESH_GTEST
#include "mesh.h"

TEST(meshtests, mesh) {
  Mesh mesh;
  mesh.load("assets/models/cube/cube.obj");

  // positions
  ASSERT_EQ(mesh.m_num_vertices, 36);
  ASSERT_EQ(mesh.mp_vertices[0].m_position, Vec3(1, -1, 1));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_normal, Vec3(-1, 1, -1));
  ASSERT_EQ(mesh.mp_vertices[0].m_normal, Vec3(0, -1, 0));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_normal, Vec3(0, 0, -1));
  ASSERT_EQ(mesh.mp_vertices[0].m_tex_coord, Vec2(1, 0.333333));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices].m_tex_coord, Vec2(0, 0.666667));

  mesh.unload();
}
