#include <blockallocator.h>
#include <gtest/gtest.h>
#include <importer.h>
#include <texture.h>

#include <cassert>

TEST(importertests, import_texture) {
  BlockAllocator<Texture> allocator(1);
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  Texture *tex = tex_importer.import(allocator);
  assert(tex != nullptr);
  assert(tex->m_width == 1024);
}
