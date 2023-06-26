#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"

int test_asset_type(void) {
  int mat = asset_type("some_mtl_file.mtl");
  if (mat != asset_type_MATERIAL) {
    return 1;
  }

  int mesh = asset_type("some_mesh_file.mesh");
  if (mesh != asset_type_MESH) {
    return 1;
  }

  return 0;
}

int main() {
  int result = 0;
  result += test_asset_type();
  return result;
}
