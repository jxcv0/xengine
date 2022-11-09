#include <gtest/gtest.h>
#include <importer.h>
#include <stdexcept>
#include <texture.h>
#include <model.h>

#include <cassert>

TEST(importertests, import_texture) {
  Importer<Texture> tex_importer("assets/models/cyborg/cyborg_diffuse.png");
  auto *tex = tex_importer.import();
  assert(tex != nullptr);
  assert(tex->m_width == 1024);
}

TEST(importertests, unsupported_file_exception) {
  Importer<Model> imp("unsupported_file.nocando");
  ASSERT_THROW(imp.import(),
      std::runtime_error);
}

TEST(importertests, parsing) {
  Importer<Model> imp("assets/models/cube/cube.obj");
  auto model = imp.import();
}
