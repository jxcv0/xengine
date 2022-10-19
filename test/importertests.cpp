#include <gtest/gtest.h>
#include <importer.h>
#include <poolallocator.h>
#include <texture.h>

#include <cassert>

TEST(importertests, import_texture) {
  PoolAllocator<Texture, 1> allocator;
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  Texture *tex = tex_importer.import(allocator);
  assert(tex != nullptr);
  assert(tex->m_width == 1024);
}
