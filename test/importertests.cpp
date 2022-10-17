#include <gtest/gtest.h>
#include <importer.h>
#include <texture.h>

TEST(importertests, import) {
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  auto ptr = tex_importer.import();
  ASSERT_NE(ptr, nullptr);
}
