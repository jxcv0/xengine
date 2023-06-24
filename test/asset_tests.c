#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"

int test_asset_type(void) {
  if (asset_type("some_mtl_file.mtl") != asset_type_MESH) {
    return 1;
  }

  if (asset_type("some_mtl_file.mesh") != asset_type_MATERIAL) {
    return 1;
  }

  return 0;
}

int main() {
  int result = 0;
  result += test_asset_type();
  return result;
}
