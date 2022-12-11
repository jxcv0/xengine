#include <gtest/gtest.h>

#define MESH_GTEST
#include "mesh.h"

TEST(meshtests, mesh) {
  Mesh mesh;
  mesh.load("assets/models/cube/cube.obj");

  // positions
  ASSERT_EQ(mesh.m_num_positions, 8);
  ASSERT_FLOAT_EQ(mesh.mp_positions[0].x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_positions[0].y(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_positions[0].z(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_positions[mesh.m_num_positions - 1].x(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_positions[mesh.m_num_positions - 1].y(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_positions[mesh.m_num_positions - 1].z(), -1.00000f);

  // tex_coords
  ASSERT_EQ(mesh.m_num_tex_coords, 14);
  ASSERT_FLOAT_EQ(mesh.mp_tex_coords[0].x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_tex_coords[0].y(), 0.333333f);
  ASSERT_FLOAT_EQ(mesh.mp_tex_coords[mesh.m_num_tex_coords - 1].x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_tex_coords[mesh.m_num_tex_coords - 1].y(), 0.00000f);

  // normals
  ASSERT_EQ(mesh.m_num_normals, 6);
  ASSERT_FLOAT_EQ(mesh.mp_normals[0].x(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_normals[0].y(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_normals[0].z(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_normals[mesh.m_num_normals - 1].x(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_normals[mesh.m_num_normals - 1].y(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.mp_normals[mesh.m_num_normals - 1].z(), -1.00000f);

  // indices
  ASSERT_EQ(mesh.m_num_indices, 36);
  ASSERT_EQ(mesh.mp_indices[0].m_position_idx, 1);
  ASSERT_EQ(mesh.mp_indices[0].m_tex_coord_idx, 0);
  ASSERT_EQ(mesh.mp_indices[0].m_normal_idx, 0);
  ASSERT_EQ(mesh.mp_indices[mesh.m_num_indices - 1].m_position_idx, 7);
  ASSERT_EQ(mesh.mp_indices[mesh.m_num_indices - 1].m_tex_coord_idx, 10);
  ASSERT_EQ(mesh.mp_indices[mesh.m_num_indices - 1].m_normal_idx, 5);

  mesh.gen_buffers();
  mesh.unload();
}
