#include <gtest/gtest.h>

#include "assets.h"
#include <stdlib.h>

TEST (assets_tests, asset_type)
{
  ASSERT_EQ(asset_type("some_mtl_file.tex"), asset_type_TEXTURE);
	ASSERT_EQ(asset_type("some_mesh_file.mesh"), asset_type_MESH);
}

TEST (assets_tests, load_texture)
{
  /* relative path from repo root */
  struct texture *tex = load_texture ("testassets/textures/default_diffuse.png", malloc);
  ASSERT_NE (tex->data, nullptr);
  ASSERT_EQ (tex->sizeinfo.width, 2048);
  ASSERT_EQ (tex->sizeinfo.height, 2048);
  ASSERT_EQ (tex->sizeinfo.nchannels, 3);
  ASSERT_EQ (tex->data[0], 255);
  ASSERT_EQ (tex->data[1], 0);
  ASSERT_EQ (tex->data[2], 195);
}

TEST (assets_tests, load_mesh)
{
  struct model model = load_meshes ("testassets/meshes/cube.obj", malloc);
  ASSERT_EQ (model.nmeshes, 1);
  ASSERT_EQ (model.meshes[0].nvertices, 8);
  ASSERT_EQ (model.meshes[0].nindices, 36);
  // ASSERT_FLOAT_EQ (m.vertices[0], 36);
}
