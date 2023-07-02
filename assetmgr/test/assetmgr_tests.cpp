#include <gtest/gtest.h>

#include "assetmgr.h"

TEST(assetmgr_tests, asset_type) {
  ASSERT_EQ(asset_type("some_mtl_file.mtl"), asset_type_MATERIAL);
	ASSERT_EQ(asset_type("some_mesh_file.mesh"), asset_type_MESH);
}
