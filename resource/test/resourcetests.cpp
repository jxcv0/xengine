#include <gtest/gtest.h>
#include <mesh.h>
#include <resource.h>
#include <texture.h>

TEST(resourcestests, texture) {
  Resource<Material> material_res("assets/models/cyborg/cyborg_diffuse.png");
  auto tex = material_res.get();
  ASSERT_NE(tex, nullptr);
  ASSERT_EQ(material_res.get()->m_map_diffuse.m_width, 1024);
}

// TODO
// TEST(resourcestests, mesh) {
// Resource<Mesh> mesh_resource("assets/models/cube/cube.obj");
// auto mesh = mesh_resource.get();
// }
