#include <gtest/gtest.h>

#include "mesh.h"

TEST(meshtests, mesh) {
  struct mesh mesh = mesh_load("assets/models/cube/cube.obj");

  ASSERT_EQ(mesh.m_num_vertices, 36);

  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[0], 1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[1], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[2], 1);

  auto n = mesh.m_num_vertices - 1;
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[0], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[1], 1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[2], -1);

  /*

  ASSERT_EQ(mesh.mp_vertices[0].m_tex_coord, Vec2(1, 0.333333));
  ASSERT_EQ(mesh.mp_vertices[mesh.m_num_vertices - 1].m_tex_coord,
            Vec2(0, 0.666667));
            */

  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[0], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[1], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[2], 0); 

  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[0], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[1], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[2], -1);

  mesh_unload(&mesh);
}
