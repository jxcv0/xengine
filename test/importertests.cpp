#include <blockallocator.h>
#include <gtest/gtest.h>
#include <importer.h>
#include <texture.h>

#include <cassert>

TEST(importertests, import_texture) {
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  Texture *tex = tex_importer.import();
  assert(tex != nullptr);
  assert(tex->m_width == 1024);
}
