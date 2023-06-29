#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "test.h"

int main() {
  ASSERT_TRUE(asset_type("some_mtl_file.mtl") == asset_type_MATERIAL);
  ASSERT_TRUE(asset_type("some_mesh_file.mesh") == asset_type_MESH);
  test_end();
  return 0;
}
