#include <gtest/gtest.h>

#include "mesh.h"

TEST(meshtests, load_mesh) {
  struct mesh mesh = load_mesh("assets/models/cube/cube.obj");

  ASSERT_EQ(mesh.m_num_vertices, 36);

  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[0], 1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[1], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_position[2], 1);

  auto n = mesh.m_num_vertices - 1;
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[0], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[1], 1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_position[2], -1);

  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_tex_coord[0], 1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_tex_coord[1], 0.333333);

  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_tex_coord[0], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_tex_coord[1], 0.666667);

  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[0], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[1], -1);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[0].m_normal[2], 0); 

  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[0], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[1], 0);
  ASSERT_FLOAT_EQ(mesh.mp_vertices[n].m_normal[2], -1);

  unload_mesh(&mesh);
}

TEST(meshtests, load_material) {
  struct material mat = load_material("assets/models/cube/cube.obj");

  ASSERT_FLOAT_EQ(mat.m_specular_exponent, 359.999993);

  ASSERT_FLOAT_EQ(mat.m_ambient_color[0], 1);
  ASSERT_FLOAT_EQ(mat.m_ambient_color[1], 1);
  ASSERT_FLOAT_EQ(mat.m_ambient_color[2], 1);

  ASSERT_FLOAT_EQ(mat.m_diffuse_color[0], 0.8);
  ASSERT_FLOAT_EQ(mat.m_diffuse_color[1], 0.8);
  ASSERT_FLOAT_EQ(mat.m_diffuse_color[2], 0.8);

  ASSERT_FLOAT_EQ(mat.m_specular_color[0], 0.5);
  ASSERT_FLOAT_EQ(mat.m_specular_color[1], 0.5);
  ASSERT_FLOAT_EQ(mat.m_specular_color[2], 0.5);
}
