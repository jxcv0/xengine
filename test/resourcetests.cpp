#include <gtest/gtest.h>
#include <resource.h>
#include <texture.h>

TEST(resourcestests, resource_test) {
  Resource<Texture> tex_resource("assets/models/cyborg/cyborg_diffuse.png");
  auto tex = tex_resource.get();
  ASSERT_NE(tex, nullptr);
  ASSERT_EQ(tex->m_width, 1024);
}
