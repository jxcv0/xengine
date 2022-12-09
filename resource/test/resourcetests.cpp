#include <gtest/gtest.h>
#include <texture.h>

#include "mesh.h"
#include "resource.h"

TEST(resourcestests, mesh_resource) {
  Resource<Mesh> res("assets/models/cube/cube.obj");
  auto m = res.get();
  ASSERT_NE(m, nullptr);
}
