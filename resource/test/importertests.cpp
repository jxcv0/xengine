#include <gtest/gtest.h>
#include <importer.h>
#include <model.h>
#include <texture.h>

#include <cassert>
#include <stdexcept>

TEST(importertests, unsupported_file_exception) {
  Importer<Model> imp("unsupported_file.nocando");
  ASSERT_THROW(imp.import(), std::runtime_error);
}

TEST(importer_impltests, obj_parsing) {
  Mesh mesh;
  import_impl::import(&mesh, "assets/models/cube/cube.obj");

  // positions
  ASSERT_FLOAT_EQ(mesh.m_positions.front().x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_positions.front().y(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_positions.front().z(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_positions.back().x(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_positions.back().y(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_positions.back().z(), -1.00000f);

  // tex_coords
  ASSERT_FLOAT_EQ(mesh.m_tex_coords.front().x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_tex_coords.front().y(), 0.333333f);
  ASSERT_FLOAT_EQ(mesh.m_tex_coords.back().x(), 1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_tex_coords.back().y(), 0.00000f);

  // normals
  ASSERT_FLOAT_EQ(mesh.m_normals.front().x(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.m_normals.front().y(), -1.00000f);
  ASSERT_FLOAT_EQ(mesh.m_normals.front().z(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.m_normals.back().x(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.m_normals.back().y(), 0.00000f);
  ASSERT_FLOAT_EQ(mesh.m_normals.back().z(), -1.00000f);

  // indices
  ASSERT_EQ(mesh.m_indices.front().m_position_idx, 1);
  ASSERT_EQ(mesh.m_indices.front().m_tex_coord_idx, 0);
  ASSERT_EQ(mesh.m_indices.front().m_normal_idx, 0);
  ASSERT_EQ(mesh.m_indices.back().m_position_idx, 7);
  ASSERT_EQ(mesh.m_indices.back().m_tex_coord_idx, 10);
  ASSERT_EQ(mesh.m_indices.back().m_normal_idx, 5);
}

TEST(import_impltests, mtl_parsing) {
  Material material;
  import_impl::import(&material, "assets/models/cyborg/cyborg.mtl");
  ASSERT_FLOAT_EQ(material.m_specular_exp, 92.15686f);
}
