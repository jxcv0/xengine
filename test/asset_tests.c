#include <assert.h>

#include "assets.h"
#include "test.h"

void tst_load_obj(void) {
  TEST();
  struct geometry geom;
  assert(load_obj_file(&geom, NULL, "assets/meshes/test_cube.obj") != 0);
  assert(geom.num_vertices == 8);
  assert(geom.num_indices == 12);
}

int main() {
  TEST_BEGIN();
  tst_load_obj();
  TEST_END();
  return 0;
}
