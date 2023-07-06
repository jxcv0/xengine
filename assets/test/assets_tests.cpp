#include <gtest/gtest.h>

#include "assets.h"

TEST(assetmgr_tests, asset_type) {
  ASSERT_EQ(asset_type("some_mtl_file.tex"), asset_type_TEXTURE);
	ASSERT_EQ(asset_type("some_mesh_file.mesh"), asset_type_MESH);
}
