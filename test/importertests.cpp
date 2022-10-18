#include <blockallocator.h>
#include <gtest/gtest.h>
#include <importer.h>
#include <texture.h>

TEST(importertests, import_texture) {
  BlockAllocator<Texture> allocator(1);
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  Texture *ptr = tex_importer.import(allocator);
  ASSERT_TRUE(ptr);
}
