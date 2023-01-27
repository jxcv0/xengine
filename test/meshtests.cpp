#include <gtest/gtest.h>

#include "mesh.h"

TEST(meshtests, load_mesh) {
  struct mesh mesh = load_mesh("assets/models/cube/cube.model");

  ASSERT_EQ(mesh.m_num_vertices, 24);
  ASSERT_EQ(mesh.m_num_indices, 36);
}

/*
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
*/
