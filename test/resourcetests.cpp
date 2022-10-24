#include <gtest/gtest.h>
#include <mesh.h>
#include <resource.h>
#include <texture.h>

TEST(resourcestests, texture) {
  Resource<Texture> tex_resource("assets/models/cyborg/cyborg_diffuse.png");
  auto tex = tex_resource.get();
  ASSERT_NE(tex, nullptr);
  ASSERT_EQ(tex->m_width, 1024);
}

TEST(resourcestests, mesh) {
  Resource<Mesh> mesh_resource("assets/models/cube.obj");
  auto mesh = mesh_resource.get();
}
