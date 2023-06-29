#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "test.h"

int main(int argc, char **argv) {
  test_start(argc, argv);
  ASSERT_TRUE(asset_type("some_mtl_file.mtl") == asset_type_MATERIAL);
  ASSERT_TRUE(asset_type("some_mesh_file.mesh") == asset_type_MESH);
  test_end();
  return 0;
}
