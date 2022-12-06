#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstring>
#include <string>

#include "mmapfile.h"

TEST(mmapfiletests, mmapfile) {
  MmapFile file("assets/models/cube/cube.obj");
  ASSERT_TRUE(file.valid());
  auto view = file.get_view();
  auto sub = view.substr(0, view.find('\n'));
  const std::string expected = "# Blender v2.76 (sub 0) OBJ File: ''";
  auto size = sub.size();
  char actual[size];
  strncpy(actual, sub.data(), size);
  actual[size] = '\0';
  ASSERT_STREQ(actual, expected.c_str());
}
